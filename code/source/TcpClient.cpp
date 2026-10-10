#include "TcpClient.h"
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

void CTcpClient::InitSocket()
{
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
    {
        return;
    }
}

void CTcpClient::CleanupSocket()
{
    WSACleanup();
}

CTcpClient::CTcpClient()
{
	m_socket = -1;
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_bConnected = false;
}

CTcpClient::~CTcpClient()
{
	Close();
}

bool CTcpClient::ConnectToServer(const std::string strIp, const int port)
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, strIp.c_str(), &serverAddr.sin_addr) != 1)
    {
		m_bConnected = false;
        return false;
    }

    // ---------- 4. 连接服务端 ----------
    if (connect(m_socket, reinterpret_cast<sockaddr*>(&serverAddr),
        sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "connect() 失败, 错误码: " << WSAGetLastError() << "\n";
        //closesocket(sock);
        //WSACleanup();
		m_bConnected = false;
        return false;
    }
    m_bConnected = true;
	return true;
}

bool CTcpClient::SendData(const char* data, const int length)
{
	const int timeoutMs = 5000; // 超时时间，单位毫秒
	int totalLen = length;
    if (IsConnected())
    {
        u_long mode = 1;   // 1 = 非阻塞
        ioctlsocket(m_socket, FIONBIO, &mode);
        if (data == nullptr || totalLen <= 0)
        {
            return false;
        }

        int sent = 0;
        DWORD startTick = GetTickCount();

        while (sent < totalLen)
        {
            // 计算本次 select 允许等待的剩余时间（整体超时）
            DWORD elapsed = GetTickCount() - startTick;
            if (elapsed >= static_cast<DWORD>(timeoutMs))
            {
                return false;
            }

            timeval tv;
            DWORD remain = static_cast<DWORD>(timeoutMs) - elapsed;
            tv.tv_sec = static_cast<long>(remain / 1000);
            tv.tv_usec = static_cast<long>((remain % 1000) * 1000);

            fd_set writeSet, exceptSet;
            FD_ZERO(&writeSet);
            FD_ZERO(&exceptSet);
            FD_SET(m_socket, &writeSet);
            FD_SET(m_socket, &exceptSet);

            // Windows 下第一个参数被忽略，传 0 即可
            int ret = select(0, nullptr, &writeSet, &exceptSet, &tv);

            if (ret == SOCKET_ERROR)
            {
                return false;
            }
            if (ret == 0)
            {
                return false;
            }
            if (FD_ISSET(m_socket, &exceptSet))
            {
                return false;
            }
            if (!FD_ISSET(m_socket, &writeSet))
            {
				//We shoun't reach here, but just in case, we continue to the next iteration.
                continue;   // 理论上不会走到，容错
            }

            // 关键：不切分，不指定大小，剩余多少就交给 send 多少。
            // send 返回值就是本次真正写出去的字节数。
            int n = send(m_socket, data + sent, totalLen - sent, 0);

            if (n == SOCKET_ERROR)
            {
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                {
                    // 非阻塞下本次没写进去，下一轮 select 后再试
                    continue;
                }
                return false;
            }
            if (n == 0)
            {
                //std::cerr << "send() 返回 0，对端可能已关闭\n";
                return false;
            }

            sent += n;
            //std::cout << "本轮 send 实际写入 " << n
            //    << " 字节，累计 " << sent
            //    << " / " << totalLen << "\n";
        }

        //std::cout << "发送完成，共 " << totalLen << " 字节\n";
        //return 0;
    }
	return true;
}

bool CTcpClient::ReadData(char* data,int* length)
{
    if (IsConnected())
    {
        const int timeoutMs = 5000; // 超时时间，单位毫秒
        int totalLen = *length;

        u_long mode = 1;   // 1 = 非阻塞
        ioctlsocket(m_socket, FIONBIO, &mode);
        if (data == nullptr || *length <= 0 || timeoutMs <= 0)
        {
            return false;
        }

        const int maxLen = *length;

        int total = 0;
        DWORD startTick = GetTickCount();

        bool bTimeOut = false;
        while (total < maxLen)
        {
            // 计算整体剩余超时时间
            DWORD elapsed = GetTickCount() - startTick;
            if (elapsed >= static_cast<DWORD>(timeoutMs))
            {
                if (bTimeOut)
                {
                    bTimeOut = true;
                }
                return total;   // 超时，返回已读到的数据
            }

            DWORD remain = static_cast<DWORD>(timeoutMs) - elapsed;

            timeval tv;
            tv.tv_sec = static_cast<long>(remain / 1000);
            tv.tv_usec = static_cast<long>((remain % 1000) * 1000);

            fd_set readSet, exceptSet;
            FD_ZERO(&readSet);
            FD_ZERO(&exceptSet);
            FD_SET(m_socket, &readSet);
            FD_SET(m_socket, &exceptSet);

            // Windows 下 select 第一个参数被忽略，传 0 即可
            int ret = select(0, &readSet, nullptr, &exceptSet, &tv);

            if (ret == SOCKET_ERROR)
            {
                //std::cerr << "select() 失败，错误码: "
                //    << WSAGetLastError() << "\n";
                return false;
            }
            if (ret == 0)
            {
                if (bTimeOut)
                {
                    bTimeOut = true;
                }
                *length = total;  // 返回已读到的数据长度
                return true;
            }
            if (FD_ISSET(m_socket, &exceptSet))
            {
                //std::cerr << "套接字出现异常\n";

                return false;
            }
            if (!FD_ISSET(m_socket, &readSet))
            {
                continue;       // 容错
            }

            // 关键：不预设每次读多少，剩余多少就交给 recv 多少。
            // recv 返回多少就是本次实际读到的字节数。
            int n = recv(m_socket, data + total, maxLen - total, 0);

            if (n == SOCKET_ERROR)
            {
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK)
                {
                    // 非阻塞下本次没读到，回到 select 继续等
                    continue;
                }
                //std::cerr << "recv() 失败，错误码: " << err << "\n";
                *length = total;
                return false;
            }
            if (n == 0)
            {
                // 对端正常关闭，返回已读到的数据
                // 如果 total == 0，调用者可通过 timedOut == false 判断连接关闭
                *length = total;
                return false;
            }

            total += n;
            //std::cout << "本轮 recv 实际读到 " << n
            //    << " 字节，累计 " << total
            //    << " / " << maxLen << "\n";
        }
    }
	return false;
}

void CTcpClient::Close()
{
	if (m_socket != -1)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_socket = -1;
	}
}

bool CTcpClient::IsConnected()
{
	return m_bConnected;
}