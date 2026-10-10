#include "LocalTcpGenerator.h"
#include "TcpClient.h"
#include "CommonMsg.h"
#include <memory>
std::unique_ptr<CTcpClient> g_tcpClient = nullptr;
CLocalTcpGenerator::CLocalTcpGenerator()
{
	CTcpClient::InitSocket();
	g_tcpClient = std::make_unique<CTcpClient>();
	std::string strIp = "127.0.0.1";
	int port = 8001;
	if (g_tcpClient->ConnectToServer(strIp, port))
	{

	}
}

CLocalTcpGenerator::~CLocalTcpGenerator()
{
	g_tcpClient.reset();
	g_tcpClient = nullptr;
	CTcpClient::CleanupSocket();
}

std::vector<std::string> CLocalTcpGenerator::Generate(
	const std::string& input,
	std::size_t maxCandidates)
{
	std::vector<std::string> candidates;
	if(g_tcpClient != nullptr)
	{
		std::string strClientId = "test";
		{
			KeepAliveReqMsg reqMsg(strClientId);
			{
				TransBaseMsg_t transMsg(reqMsg.GetMsgType(), reqMsg.ToString());
				g_tcpClient->SendData(transMsg.GetData(), transMsg.GetSize());
			}
			{
				int memoryLength = 2048;
				char* pData = new char[memoryLength];
				if (nullptr != pData)
				{
					memset(pData, 0, memoryLength);
					int readLength = memoryLength;
					g_tcpClient->ReadData(pData, &readLength);
					TransBaseMsg_t transMsg(pData);
					if (transMsg.GetType() == MessageType::KeepAliveRsp_Type)
					{
						KeepAliveRspMsg rspMsg;
						if (rspMsg.FromString(transMsg.to_string()))
						{
						}
					}
				}
				delete[] pData;
			}
		}
		{

			std::string strInput(input.begin(), input.end());
			CandidateGeneratorReqMsg reqMsg(strClientId, strInput);
			{
				TransBaseMsg_t transMsg(reqMsg.GetMsgType(), reqMsg.ToString());
				g_tcpClient->SendData(transMsg.GetData(), transMsg.GetSize());
			}

			int memoryLength = 2048;
			char* pData = new char[memoryLength];
			if (nullptr != pData)
			{
				memset(pData, 0, memoryLength);
				int readLength = memoryLength;
				g_tcpClient->ReadData(pData, &readLength);
				TransBaseMsg_t transMsg(pData);
				if (transMsg.GetType() == MessageType::CandidateRsp_Type)
				{
					CandidateGeneratorRspMsg rspMsg;
					if (rspMsg.FromString(transMsg.to_string()))
					{
						auto candidateList = rspMsg.GetCandidates();
					}
				}
			}
			delete[] pData;
		}
	}
	//
	//
	//
	return candidates;
}


