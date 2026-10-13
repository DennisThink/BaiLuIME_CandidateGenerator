#include "RimeIceDictionary.h"
#include "TextFile.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
void CRimeIceDictionary::PrintDictElem(const DictElem_t& elem)
{
    std::cout.clear();
    std::cout <<"PrintDictElem Chinese Part: ";
    std::cout << elem._strChinese;
    std::cout << " PinYin Part:";
    for (auto item : elem._strPinYin)
    {
        std::cout << item;
        std::cout << L" ";
    }
    std::cout << " Frequency Part:";
    std::cout << elem._frequency << std::endl;
    //std::flush(std::wcout);
    return;
}
std::string CRimeIceDictionary::DictElemToString(const DictElem_t& elem)
{
    std::string strResult;
    strResult += elem._strChinese;
    strResult += ":";
    auto itemStart = elem._strPinYin.begin();
    do
    {
        strResult += *itemStart;
        itemStart++;
        if (itemStart == elem._strPinYin.end())
        {
            break;
        }
        strResult += "-";
    } while (itemStart != elem._strPinYin.end());

    strResult += ":";
    strResult += std::to_string(elem._frequency);
    return strResult;
}

DictElem_t CRimeIceDictionary::StringToDictElem(const std::string& strDict)
{
    DictElem_t resultElem;
    
    auto itemStart = strDict.begin();
    while ((itemStart != strDict.end()) && (*itemStart != L':'))
    {
        resultElem._strChinese += *itemStart;
        itemStart++;
    }
    itemStart++;//Skip ":"

    std::string strSinglePinYin;
    while ((itemStart != strDict.end()) && (*itemStart != L':'))
    {
        if (*itemStart == '-')
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
bool CRimeIceDictionary::ReadDataFromFile(const std::string strFileName)
{
	enc::TextFile file(strFileName);
    m_allDictElem.clear();
	auto& allLines = file.lines();
	for(const auto& line : allLines) {
        DictElem_t elem = StringToDictElem(line);
        m_allDictElem.push_back(elem);
    }
	return true;
}

bool CRimeIceDictionary::WriteDataToFile(const std::string strFileName)
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
            std::string text = DictElemToString(dictItem);
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