#include "RimeCandidateGenerator.h"
#include "RimeIceDictionary.h"
#include <memory>
static std::unique_ptr<CRimeIceDictionary> g_dictionary = nullptr;
CRimeCandiateGenerator::CRimeCandiateGenerator()
{
    if (g_dictionary == nullptr)
    {
        g_dictionary = std::make_unique<CRimeIceDictionary>();
    }
    if (g_dictionary != nullptr)
    {
        g_dictionary->ReadDataFromFile(L"8105.dict.txt");
    }
    if (g_dictionary != nullptr)
    {
        auto dictData = g_dictionary->GetAllData();
        std::wstring strKey;
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
                std::vector<std::wstring> chineseValue;
                chineseValue.push_back(dictItem._strChinese);
                m_simpleDirectory.insert({ strKey,chineseValue });
            }
        }
    }
}

CRimeCandiateGenerator::~CRimeCandiateGenerator()
{
    if (g_dictionary != nullptr)
    {
        g_dictionary = nullptr;
    }
}
std::vector<std::wstring> CRimeCandiateGenerator::Generate(
    const std::wstring& input,
    std::size_t maxCandidates)
{
    std::vector<std::wstring> result;
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