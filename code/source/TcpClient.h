#ifndef _TCP_CLIENT_GENERATOR_H_
#define _TCP_CLIENT_GENERATOR_H_
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <string>
class CTcpClient
{
public:
	static void InitSocket();
	static void CleanupSocket();
public:
CTcpClient();
virtual ~CTcpClient();
bool ConnectToServer(const std::string strIp,const int port);
bool SendData(const char * data,const int length);
bool ReadData(char * data,int* length);
private:
	std::string m_strIp;
	int m_port;
	bool m_bConnected;
	bool IsConnected();
	void Close();
	SOCKET m_socket;
};
#endif