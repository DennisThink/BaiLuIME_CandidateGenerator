#include "RimeIceDictionary.h"
#include <sstream>
#include <iostream>
#include <fstream>
void CRimeIceDictionary::PrintDictElem(const DictElem_t& elem)
{
    std::wcout.clear();
    std::wcout <<L"PrintDictElem Chinese Part: ";
    std::wcout << elem._strChinese;
    std::wcout << L" PinYin Part:";
    for (auto item : elem._strPinYin)
    {
        std::wcout << item;
        std::wcout << L" ";
    }
    std::wcout << L" Frequency Part:";
    std::wcout << elem._frequency << std::endl;
    //std::flush(std::wcout);
    return;
}
std::wstring CRimeIceDictionary::DictElemToString(const DictElem_t& elem)
{
    std::wstring strResult;
    strResult += elem._strChinese;
    strResult += L":";
    auto itemStart = elem._strPinYin.begin();
    do
    {
        strResult += *itemStart;
        itemStart++;
        if (itemStart == elem._strPinYin.end())
        {
            break;
        }
        strResult += L"-";
    } while (itemStart != elem._strPinYin.end());

    strResult += L":";
    strResult += std::to_wstring(elem._frequency);
    return strResult;
}

DictElem_t CRimeIceDictionary::StringToDictElem(const std::wstring& strDict)
{
    DictElem_t resultElem;
    
    auto itemStart = strDict.begin();
    while ((itemStart != strDict.end()) && (*itemStart != L':'))
    {
        resultElem._strChinese += *itemStart;
        itemStart++;
    }
    itemStart++;//Skip ":"

    std::wstring strSinglePinYin;
    while ((itemStart != strDict.end()) && (*itemStart != L':'))
    {
        if (*itemStart == L'-')
        {
            resultElem._strPinYin.push_back(strSinglePinYin);
            strSinglePinYin.clear();
        }
        else
        {
            strSinglePinYin += *itemStart;
        }
        itemStart++;
    }
    resultElem._strPinYin.push_back(strSinglePinYin);
    strSinglePinYin.clear();
    itemStart++;//Skip ":"
    std::wstring strNumber;
    while (itemStart != strDict.end())
    {
        strNumber += *itemStart;
        itemStart++;
    }
    resultElem._frequency = std::stoll(strNumber);
    return resultElem;
}
bool CRimeIceDictionary::ReadDataFromFile(const std::wstring strFileName)
{
    // 设置全局 locale 为 UTF-8，以便正确输出宽字符
    std::locale::global(std::locale(""));
    std::ifstream ifs(strFileName, std::ios::binary);
    if (!ifs) 
    { 
        std::cerr << "无法打开文件\n"; 
        return false; 
    }
   
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
    m_allDictElem.clear();
    for (wchar_t c : ws) {
        if (c == L'\n') {
           
            if (!line.empty() && line.back() == L'\r')
            {
               
                line.pop_back();
            }
           
            //std::wcout << line << std::endl;
            DictElem_t elem = StringToDictElem(line);
            m_allDictElem.push_back(elem);
            //PrintDictElem(elem);
            //std::wcout << L"PinYin: " << dictLine._strPinYin << "-------Chinese:" << dictLine._strChinese << std::endl;
           
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
	return false;
}

bool CRimeIceDictionary::WriteDataToFile(const std::wstring strFileName)
{
    bool bWriteBoom = true;
    bool bLittleEndian = true;
    std::ofstream ofs(strFileName, std::ios::binary | std::ios::trunc);
    if (!ofs) {
        return false;
    }

    // 1. 写 BOM
    if (bWriteBoom) {
        const unsigned char bomLE[2] = { 0xFF, 0xFE };
        const unsigned char bomBE[2] = { 0xFE, 0xFF };
        ofs.write(reinterpret_cast<const char*>(bLittleEndian ? bomLE : bomBE), 2);
    }

    for (auto dictItem : m_allDictElem) 
    {
        
        {
            std::wstring text = DictElemToString(dictItem);
            // 2. 按指定字节序写每个 UTF-16 code unit
            //    注意：不能直接把 u16string.data() 写出去，
            //    因为 host 的字节序不一定和目标一致（虽然 x86/ARM 都是 LE，但显式处理更安全）
            for (char16_t c : text) {
                uint16_t v = static_cast<uint16_t>(c);
                unsigned char bytes[2];
                if (bLittleEndian) {
                    bytes[0] = static_cast<unsigned char>(v & 0xFF);
                    bytes[1] = static_cast<unsigned char>((v >> 8) & 0xFF);
                }
                else {
                    bytes[0] = static_cast<unsigned char>((v >> 8) & 0xFF);
                    bytes[1] = static_cast<unsigned char>(v & 0xFF);
                }
                ofs.write(reinterpret_cast<const char*>(bytes), 2);
            }
        }
        {
            std::wstring text = L"\r\n";
            // 2. 按指定字节序写每个 UTF-16 code unit
            //    注意：不能直接把 u16string.data() 写出去，
            //    因为 host 的字节序不一定和目标一致（虽然 x86/ARM 都是 LE，但显式处理更安全）
            for (char16_t c : text) {
                uint16_t v = static_cast<uint16_t>(c);
                unsigned char bytes[2];
                if (bLittleEndian) {
                    bytes[0] = static_cast<unsigned char>(v & 0xFF);
                    bytes[1] = static_cast<unsigned char>((v >> 8) & 0xFF);
                }
                else {
                    bytes[0] = static_cast<unsigned char>((v >> 8) & 0xFF);
                    bytes[1] = static_cast<unsigned char>(v & 0xFF);
                }
                ofs.write(reinterpret_cast<const char*>(bytes), 2);
            }
        }
    }
    

    if (!ofs) {
        return false;
    }
	return true;
}

std::vector<DictElem_t> CRimeIceDictionary::GetAllData() const
{
    std::vector<DictElem_t> result = m_allDictElem;
	return result;
}
bool CRimeIceDictionary::AddDictElem(const DictElem_t& elem)
{
	return false;
}
bool CRimeIceDictionary::RemoveDictElem(const DictElem_t& elem)
{
	return false;
}