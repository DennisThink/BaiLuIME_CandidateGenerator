#ifndef _BAILU_IME_DICTIONARY_INTERFACE_H_
#define _BAILU_IME_DICTIONARY_INTERFACE_H_
#include <vector>
#include <string>
struct DictElem_t
{
	std::wstring _strChinese;
	std::vector<std::wstring> _strPinYin;
	int _frequency;
};

class IDictionary
{
public:
	IDictionary()=default;
	virtual ~IDictionary() = default;
	virtual bool ReadDataFromFile(const std::wstring strFileName) = 0;
	virtual bool WriteDataToFile(const std::wstring strFileName) = 0;
	
	virtual std::vector<DictElem_t> GetAllData() const = 0;
	virtual bool AddDictElem(const DictElem_t& elem) = 0;
	virtual bool RemoveDictElem(const DictElem_t& elem) = 0;
};
#endif