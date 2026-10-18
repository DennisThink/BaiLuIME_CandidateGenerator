#include "RimeCandidateGenerator.h"
#include "RimeIceDictionary.h"
#include <memory>
bool CRimeCandiateGenerator::ReadDataFromFile(const std::string strFileName)
{
    if (m_pDictionary != nullptr)
    {
        m_pDictionary->ReadDataFromFile(strFileName);
    }
    if (m_pDictionary != nullptr)
    {
        auto dictData = m_pDictionary->GetAllData();
        std::string strKey;
        for (auto dictItem : dictData)
        {
            strKey.clear();
            for (auto pinYinItem : dictItem._strPinYin)
            {
                strKey += pinYinItem;
            }

            auto valueItem = m_simpleDirectory.find(strKey);
            if (valueItem != m_simpleDirectory.end())
            {
                valueItem->second.push_back(dictItem._strChinese);
            }
            else
            {
                std::vector<std::string> chineseValue;
                chineseValue.push_back(dictItem._strChinese);
                m_simpleDirectory.insert({ strKey,chineseValue });
            }
        }
    }
    return true;
}

void CRimeCandiateGenerator::InitDictionary()
{
    if (m_pDictionary == nullptr)
    {
        m_pDictionary = std::make_unique<CRimeIceDictionary>();
    }
}
void CRimeCandiateGenerator::UninitDictionary()
{
    if (m_pDictionary != nullptr)
    {
        m_pDictionary = nullptr;
	}
}

CRimeCandiateGenerator::CRimeCandiateGenerator(const std::string& dictFilePath)
{
    InitDictionary();
    ReadDataFromFile(dictFilePath);
}
CRimeCandiateGenerator::CRimeCandiateGenerator()
{
    InitDictionary();
}

CRimeCandiateGenerator::~CRimeCandiateGenerator()
{
    UninitDictionary();
}

bool CRimeCandiateGenerator::IsDataReady() const
{
    return m_pDictionary != nullptr;
}
std::vector<std::string> CRimeCandiateGenerator::Generate(
    const std::string& input,
    std::size_t maxCandidates)
{
    std::vector<std::string> result;
    auto valueItem = m_simpleDirectory.find(input);
    if (valueItem != m_simpleDirectory.end())
    {
        std::size_t i = 0;
        for (auto chineseItem : valueItem->second)
        {
            if (i < maxCandidates)
            {
                result.push_back(chineseItem);
                i++;
            }
            else
            {
                break;
            }
        }
    }
    return result;
}