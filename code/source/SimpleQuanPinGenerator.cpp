#include "SimpleQuanPinGenerator.h"
#include <sstream>
#include <iostream>
#include <fstream>
CSimpleQuanPinGenerator::CSimpleQuanPinGenerator()
{
    ReadFileToMemory();
}
CSimpleQuanPinGenerator::DictLine CSimpleQuanPinGenerator::ConvertToDictLine(const std::wstring strLine)
{
    DictLine line;
    std::size_t index = 0;
    if (index < strLine.length() && strLine[index] == L'"')
    {
        index++;
    }

    while (index < strLine.length() && strLine[index] != L'"')
    {
        line._strPinYin +=std::tolower(strLine[index]);
        index++;
    }

    if (index < strLine.length() && strLine[index] == L'"')
    {
        index++;
    }

    if (index < strLine.length() && strLine[index] == L'=')
    {
        index++;
    }

    if (index < strLine.length() && strLine[index] == L'"')
    {
        index++;
    }
    while (index < strLine.length() && strLine[index] != L'"')
    {
        line._strChinese += (strLine[index]);
        index++;
    }

    return line;
}
void CSimpleQuanPinGenerator::ReadFileToMemory()
{
    // 设置全局 locale 为 UTF-8，以便正确输出宽字符
    std::locale::global(std::locale(""));

    const std::string filename = "SampleIMESimplifiedQuanPin_SMALL.txt";


    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) { std::cerr << "无法打开文件\n"; return; }

    std::string bytes((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());

    if (bytes.size() >= 2 &&
        static_cast<unsigned char>(bytes[0]) == 0xFF &&
        static_cast<unsigned char>(bytes[1]) == 0xFE) {
        bytes.erase(0, 2);
    }

    // 组装 UTF-16 LE 码元
    std::u16string u16;
    u16.resize(bytes.size() / 2);
    for (size_t i = 0; i < u16.size(); ++i) {
        u16[i] = static_cast<char16_t>(
            static_cast<unsigned char>(bytes[2 * i]) |
            (static_cast<unsigned char>(bytes[2 * i + 1]) << 8));
    }

    // 转成 wstring 再输出（wchar_t 大小平台相关）
    std::wstring ws;
    ws.reserve(u16.size());
    for (char16_t c : u16) {
        ws.push_back(static_cast<wchar_t>(c));  // 仅 BMP 字符正确
    }

    // 按行输出
    std::wstring line;
    for (wchar_t c : ws) {
        if (c == L'\n') {
            if (!line.empty() && line.back() == L'\r') line.pop_back();
            //std::wcout << line << L'\n';
            DictLine dictLine = ConvertToDictLine(line);
            //std::wcout << L"PinYin: " << dictLine._strPinYin << "-------Chinese:" << dictLine._strChinese << std::endl;
            auto item = m_simpleDirectory.find(dictLine._strPinYin);
            if(item != m_simpleDirectory.end())
            {
                item->second.push_back(dictLine._strChinese);
            }
            else
            {
                std::vector<std::wstring> result;
                result.push_back(dictLine._strChinese);
                m_simpleDirectory.insert({ dictLine._strPinYin,result });
            }
            line.clear();
        }
        else {
            line.push_back(c);
        }
    }
    if (!line.empty()) {
        if (line.back() == L'\r') line.pop_back();
        std::wcout << line << L'\n';
    }
}

std::vector<std::wstring> CSimpleQuanPinGenerator::Generate(
    const std::wstring& input,
    std::size_t maxCandidates)
{
    auto item = m_simpleDirectory.find(input);
    if (item != m_simpleDirectory.end())
    {
        return item->second;
    }

    std::vector<std::wstring> result;
    return result;
}