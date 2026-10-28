#include "LocalTcpGenerator.h"
#include "TcpClient.h"
#include "CommonMsg.h"
#include <memory>
#include <iostream>
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

bool CLocalTcpGenerator::IsDataReady() const
{
	return true;
}

CandidateResult CLocalTcpGenerator::Generate(
	const std::string& input,
	std::size_t maxCandidates)
{
	CandidateResult candidates;
	if(g_tcpClient != nullptr)
	{
		std::string strClientId = "test";
		//We use Keep alive Message for test connection state and only in test mode
#if 0
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
#endif
		{

			std::string strInput = input;
			CandidateGeneratorReqMsg reqMsg(strClientId, strInput,maxCandidates);
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
				if(readLength > sizeof(Header))
				{
					std::cout << "ReadData failed or connection closed." << std::endl;
					TransBaseMsg_t transMsg(pData);
					std::cout << "Received message type: " << static_cast<int>(transMsg.GetType()) << std::endl;
					std::cout << "Received message size: " << transMsg.GetSize() << std::endl;
					std::cout << "Received message content: " << transMsg.to_string() << std::endl;
					if (transMsg.GetType() == MessageType::CandidateRsp_Type)
					{
						CandidateGeneratorRspMsg rspMsg;
						if (rspMsg.FromString(transMsg.to_string()))
						{
							auto candidateList = rspMsg.GetCandidates();
							for (auto& item : candidateList)
							{
								CandidateValue elem;
								elem.m_nWeight = item.m_nWeight;
								elem.m_strChinese = item.m_strChinese;
								candidates.m_candidates.push_back(elem);
								//candidates.push_back(item.m_strChinese);
							}
						}
					}
				}
				delete[] pData;
			}

		}
	}
	//
	//
	//
	return candidates;
}


