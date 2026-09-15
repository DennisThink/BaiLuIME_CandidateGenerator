#ifndef _SIMPLE_QUAN_PIN_GENERATOR_H_
#define _SIMPLE_QUAN_PIN_GENERATOR_H_
#include "CandidateGenerator.h"
#include <map>
#include <string>
class CSimpleQuanPinGenerator :public CandidateGenerator
{
public:
    CSimpleQuanPinGenerator();
public:
    virtual std::vector<std::wstring> Generate(
        const std::wstring& input,
        std::size_t maxCandidates) override;
private:
    struct DictLine{
        std::wstring _strPinYin;
        std::wstring _strChinese;
    };
    DictLine ConvertToDictLine(const std::wstring strLine);
    void ReadFileToMemory();
    std::map<std::wstring, std::vector<std::wstring>> m_simpleDirectory;
};
#endif