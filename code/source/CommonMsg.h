#ifndef _COMMON_MSG_H_
#define _COMMON_MSG_H_
#include "json11.hpp"
#include <stdlib.h>
#include <string.h>
/**
 * @brief 定义消息的类型，
 *        如果您需要定义新的消息，可以直接在后面添加
 * 
 */
enum class MessageType :uint32_t
{
	Base_Type,
	//心跳请求消息
	KeepAliveReq_Type,
	//心跳回复消息
	KeepAliveRsp_Type,
	//候选词请求消息
	CandidateReq_Type,
	//候选词回复消息
	CandidateRsp_Type,
};

/**
 * @brief 消息体的头部
 * 
 */
struct Header
{
	//消息类型
	MessageType m_type;
	//消息长度
	uint32_t    m_length;
};

/**
 * @brief 用来传输消息的类型，将消息先转化为字符串，然后放入这个类型中，在实际的代码中使用std::shared_ptr来保存
 * 
 */
struct TransBaseMsg_t
{
public:
	/**
	 * @brief 获取消息类型
	 * 
	 * @return MessageType 
	 */
	MessageType GetType() const 
	{
		Header* head = reinterpret_cast<Header*>(m_data);
		return head->m_type;
	}

	/**
	 * @brief 获取消息体的长度
	 * 
	 * @return std::size_t 
	 */
	std::size_t GetSize() const
	{
		Header* head = reinterpret_cast<Header*>(m_data);
		return head->m_length;
	}

	/**
	 * @brief 获取消息的字符串内容
	 * 
	 * @return std::string 
	 */
	std::string to_string() const
	{ 
		return std::string((const char*)(m_data+sizeof(Header)),GetSize()-sizeof(Header));
	}

	/**
	 * @brief 获取消息的实际数据
	 * 
	 * @return const char* 
	 */
	const char * GetData() const 
	{
		return m_data;
	}
	
	/**
	 * @brief 使用消息类型和字符串构造传输的消息,发送消息的时候使用
	 * 
	 * @param type 消息类型
	 * @param strMsg  消息字符串
	 */
	explicit TransBaseMsg_t(const MessageType& type,const std::string& strMsg)
	{
		std::size_t strLen = strMsg.length();
		Header head;
		head.m_type = type;
		head.m_length = static_cast<uint32_t>(strLen +sizeof(head));
		m_data = new char[head.m_length];
		memset(m_data,0,head.m_length);
		memcpy(m_data,&head,sizeof(head));
		memcpy(m_data+sizeof(head),strMsg.c_str(),strLen);
		m_selfData = true;
	}
	
	/**
	 * @brief 从内存直接构造消息，接收消息的时候使用
	 * 
	 * @param data 消息接收端的内存
	 */
	explicit TransBaseMsg_t(char * data)
	{
		m_data = data;
		m_selfData = false;
	}

	virtual ~TransBaseMsg_t()
	{
		if(m_selfData)
		{
			if(nullptr != m_data)
			{
				delete m_data;
				m_data = nullptr;
			}
		}
	}
protected:
	//数据块内容
	char *   m_data;
	//是否自己保存了数据
	bool m_selfData = false;
};


/**
 * @brief Ip端口配置类
 * 
 */
struct IpPortCfg
{
	std::string m_strServerIp;
	int 		m_nPort;
	/**
	 * @brief 获取配置的字符串表示，类似于 127.0.0.1:80
	 * 
	 * @return std::string 
	 */
	std::string to_string()
	{
		return m_strServerIp+":"+std::to_string(m_nPort);
	}

	/**
	 * @brief 检查配置是否有效
	 * 
	 * @return true 配置有效
	 * @return false 配置无效
	 */
	bool Valid()
	{
		if(m_strServerIp.empty())
		{
			return false;
		}		
		if(m_nPort<1024)
		{
			return false;
		}
		return true;
	}
};

/**
 * @brief 基础消息类型，所有的消息从该类型继承
 * 
 */
struct BaseMsg
{
protected:
	MessageType m_type;
public:
	/**
	 * @brief 获取消息类型
	 * 
	 * @return MessageType 
	 */
	MessageType GetMsgType()
	{
		return m_type;
	}

	explicit BaseMsg()
	{
		m_type=MessageType::Base_Type;
	}
	
	/**
	 * @brief 消息序列化,需要子类继承
	 * 
	 * @return std::string 序列化以后的字符串
	 */
	virtual std::string ToString() const
	{
		return "";
	}

	/**
	 * @brief 消息反序列化，需要子类继承
	 * 
	 * @param strJson 待反序列化的子串
	 * @return true 反序列化成功
	 * @return false 反序列化失败
	 */
    virtual bool FromString(const std::string& strJson)
	{
		if(strJson.empty())
		{
			return false;
		}
		return true;
	}
};


/**
 * @brief 心跳请求消息
 * 
 */
struct KeepAliveReqMsg final:public BaseMsg
{
	std::string m_strClientId;
public:
	explicit KeepAliveReqMsg()
	{
		m_type = MessageType::KeepAliveReq_Type;
		m_strClientId= "";
	}
	explicit KeepAliveReqMsg(const std::string& clientId)
	{
		m_type = MessageType::KeepAliveReq_Type;
		m_strClientId=clientId;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json clientObj = Json::object({
				{"ClientID",m_strClientId}
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if(!err.empty())
		{
			return false;
		}
		
		if(json["ClientID"].is_string())
		{
			m_strClientId = json["ClientID"].string_value();
		}
		else 
		{
			return false;
		}
		if(m_strClientId.empty())
		{
			return false;
		}
		return true;
	}
};

/**
 * @brief 心跳回复消息
 * 
 */
class KeepAliveRspMsg final:public BaseMsg
{
private:
	std::string m_strClientId;
public:
	explicit KeepAliveRspMsg()
	{
		m_type = MessageType::KeepAliveRsp_Type;
		m_strClientId="";
	}
	explicit KeepAliveRspMsg(const std::string& clientId)
	{
		m_type = MessageType::KeepAliveRsp_Type;
		m_strClientId=clientId;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json clientObj = Json::object({
				{"ClientID",m_strClientId}
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if(!err.empty())
		{
			return false;
		}

		if(json["ClientId"].is_string())
		{
			m_strClientId = json["ClientId"].string_value();
		}
		else 
		{
			return false;
		}
		if(m_strClientId.empty())
		{
			return false;
		}
		return true;
	}
};

class CandidateGeneratorReqMsg final :public BaseMsg
{
private:
	std::string m_strClientId;
	std::string m_strUserInput;
public:
	explicit CandidateGeneratorReqMsg()
	{
		m_type = MessageType::CandidateReq_Type;
		m_strClientId = "";
	}
	explicit CandidateGeneratorReqMsg(const std::string& clientId,const std::string userInput)
	{
		m_type = MessageType::CandidateReq_Type;
		m_strClientId = clientId;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json clientObj = Json::object({
				{"ClientID",m_strClientId},
				{"UserInput",m_strUserInput}
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if (!err.empty())
		{
			return false;
		}

		if (json["ClientId"].is_string())
		{
			m_strClientId = json["ClientId"].string_value();
		}
		else
		{
			return false;
		}
		if (m_strClientId.empty())
		{
			return false;
		}

		if (json["UserInput"].is_string())
		{
			m_strUserInput = json["UserInput"].string_value();
		}
		else
		{
			return false;
		}
		if (m_strUserInput.empty())
		{
			return false;
		}
		return true;
	}
};

class CandidateGeneratorRspMsg final :public BaseMsg
{
private:
	std::string m_strClientId;
	std::vector<std::string> m_vecCandidate;
public:
	explicit CandidateGeneratorRspMsg()
	{
		m_type = MessageType::CandidateRsp_Type;
		m_strClientId = "";
	}
	explicit CandidateGeneratorRspMsg(const std::string& clientId, const std::vector<std::string>& candidateList)
	{
		m_type = MessageType::CandidateRsp_Type;
		m_strClientId = clientId;
		m_vecCandidate = candidateList;
	}
	virtual std::string ToString() const override
	{
		using namespace json11;
		Json::array candidateArray;
		for(auto item:m_vecCandidate)
		{
			candidateArray.push_back(item);
		}

		Json clientObj = Json::object({
				{"ClientID",m_strClientId},
				{"CandidateList",candidateArray }
			});
		return clientObj.dump();
	}

	virtual bool FromString(const std::string& strJson) override
	{
		std::string err;
		using namespace json11;
		auto json = Json::parse(strJson, err);
		if (!err.empty())
		{
			return false;
		}

		if (json["ClientId"].is_string())
		{
			m_strClientId = json["ClientId"].string_value();
		}
		else
		{
			return false;
		}
		if (m_strClientId.empty())
		{
			return false;
		}

		if (json["CandidateList"].is_array())
		{
			auto allItems = json["CandidateList"].array_items();
			for(auto  item : allItems)
			{
				if(item.is_string())
				{
					m_vecCandidate.push_back(item.string_value());
				}
			}
		}
		else
		{
			return false;
		}
		if (m_vecCandidate.empty())
		{
			return false;
		}
		return true;
	}

	std::vector<std::string> GetCandidates() const
	{
		return m_vecCandidate;
	}
};
/**
 * @brief 服务器配置
 * 
 */
class CServerConfig
{
public:
	std::vector<IpPortCfg> m_clientCfgVec;
	IpPortCfg              m_serverCfg;
	explicit CServerConfig()
	{
		m_serverCfg.m_strServerIp = "127.0.0.1";
		m_serverCfg.m_nPort = 9000;

		m_clientCfgVec.push_back({"127.0.0.1",8000});
	}
};
#endif
