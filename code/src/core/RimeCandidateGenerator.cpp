#include "RimeCandidateGenerator.h"
#include "RimeIceDictionary.h"
#include "LevenshteinDistance.h"
#include "LongestCommonSubstringDistance.h"
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

            //SingleMap
            {
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

            //Fill Map Vector
            {
                std::size_t KEY_LEN = strKey.length();
                while (m_CandidateMaps.size() < KEY_LEN + 1)
                {
                    CandidateMap curMap;
                    m_CandidateMaps.push_back(curMap);
                }

                if (m_CandidateMaps.size() > KEY_LEN)
                {
                    auto valueItem = m_CandidateMaps[KEY_LEN].find(strKey);
                    if (valueItem != m_CandidateMaps[KEY_LEN].end())
                    {
                        valueItem->second.push_back(dictItem._strChinese);
                    }
                    else
                    {
                        std::vector<std::string> chineseValue;
                        chineseValue.push_back(dictItem._strChinese);
                        m_CandidateMaps[KEY_LEN].insert({ strKey,chineseValue });
                    }
                }
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
    /*auto valueItem = m_simpleDirectory.find(input);
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
    }*/

    std::size_t INPUT_LEN = input.length();
    if (m_CandidateMaps.size() > INPUT_LEN)
    {
        auto& curMap = m_CandidateMaps[INPUT_LEN];
        auto item = curMap.find(input);
        if (item != curMap.end())
        {
            result = item->second;
        }
    }

    //Distance Match
    if (m_CandidateMaps.size() > INPUT_LEN)
    {
        if (result.empty())
        {
            //CLevenshteinDistance distanceUtil;
            CLongestCommonSubstringDistance distanceUtil;
            std::size_t Count = m_CandidateMaps.size();
            int distance = 0;
            for (std::size_t index = INPUT_LEN; index < Count; index++)
            {
                auto& curMap = m_CandidateMaps[index];
                for (auto& item : curMap)
                {
                    int curDistance = distanceUtil.GetDistance(input, item.first);
                    if (curDistance > 0)
                    {
                        if (curDistance > distance)
                        {
                            distance = curDistance;
                            result = item.second;
                        }
                        else if (curDistance == distance)
                        {
                            result.insert(result.end(), item.second.begin(), item.second.end());
                        }
                    }
                }
            }
        }
    }
    return result;
}