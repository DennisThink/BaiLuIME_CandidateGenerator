#include "SimpleQuanPinGenerator.h"
#include "TextFile.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
CSimpleQuanPinGenerator::CSimpleQuanPinGenerator(const std::string& dictFilePath)
{
	ReadFileToMemory(dictFilePath);
}
CSimpleQuanPinGenerator::CSimpleQuanPinGenerator()
{
    //ReadFileToMemory();
}
CSimpleQuanPinGenerator::DictLine CSimpleQuanPinGenerator::ConvertToDictLine(const std::string strLine)
{
    DictLine line;
    std::size_t index = 0;
    if (index < strLine.length() && strLine[index] == '"')
    {
        index++;
    }

    while (index < strLine.length() && strLine[index] != '"')
    {
        line._strPinYin +=std::tolower(strLine[index]);
        index++;
    }

    if (index < strLine.length() && strLine[index] == '"')
    {
        index++;
    }

    if (index < strLine.length() && strLine[index] == '=')
    {
        index++;
    }

    if (index < strLine.length() && strLine[index] == '"')
    {
        index++;
    }
    while (index < strLine.length() && strLine[index] != '"')
    {
        line._strChinese += (strLine[index]);
        index++;
    }

    return line;
}
void CSimpleQuanPinGenerator::ReadFileToMemory(const std::string& strFileName)
{
	enc::TextFile file(strFileName);


	const auto& lines = file.lines();
	for (const auto& line : lines) {
        DictLine dictLine = ConvertToDictLine(line);
        auto item = m_simpleDirectory.find(dictLine._strPinYin);
        if(item != m_simpleDirectory.end())
        {
            item->second.push_back(dictLine._strChinese);
        }
        else
        {
            std::vector<std::string> result;
            result.push_back(dictLine._strChinese);
            m_simpleDirectory.insert({ dictLine._strPinYin,result });
        }
    }
}

std::vector<std::string> CSimpleQuanPinGenerator::Generate(
    const std::string& input,
    std::size_t maxCandidates)
{
    auto item = m_simpleDirectory.find(input);
    if (item != m_simpleDirectory.end())
    {
        return item->second;
    }

    std::vector<std::string> result;
    return result;
}

bool CSimpleQuanPinGenerator::IsDataReady() const
{
	return !m_simpleDirectory.empty();
}