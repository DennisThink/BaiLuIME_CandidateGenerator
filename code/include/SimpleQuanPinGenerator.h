#ifndef _SIMPLE_QUAN_PIN_GENERATOR_H_
#define _SIMPLE_QUAN_PIN_GENERATOR_H_
#include "CandidateGenerator.h"
#include <map>
#include <string>
class CSimpleQuanPinGenerator :public CandidateGenerator
{
public:
	CSimpleQuanPinGenerator(const std::string& dictFilePath);
    CSimpleQuanPinGenerator();
public:
    virtual std::vector<std::string> Generate(
        const std::string& input,
        std::size_t maxCandidates) override;
    virtual bool IsDataReady() const override;
private:
    struct DictLine{
        std::string _strPinYin;
        std::string _strChinese;
    };
    DictLine ConvertToDictLine(const std::string strLine);
    void ReadFileToMemory(const std::string& strFileName);
    std::map<std::string, std::vector<std::string>> m_simpleDirectory;
};
#endif