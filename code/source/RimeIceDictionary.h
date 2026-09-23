#ifndef _BAILU_IME_RIME_ICE_DICTIONARY_H_
#define _BAILU_IME_RIME_ICE_DICTIONARY_H_
#include "IDictionaryInterface.h"
class CRimeIceDictionary :public IDictionary
{
public:
	virtual bool ReadDataFromFile(const std::wstring strFileName) override;
	virtual bool WriteDataToFile(const std::wstring strFileName) override;

	virtual std::vector<DictElem_t> GetAllData() const override;
	virtual bool AddDictElem(const DictElem_t& elem) override;
	virtual bool RemoveDictElem(const DictElem_t& elem) override;
private:
	void PrintDictElem(const DictElem_t& elem);
	std::wstring DictElemToString(const DictElem_t& elem);
	DictElem_t StringToDictElem(const std::wstring& strDict);

private:
	std::vector<DictElem_t> m_allDictElem;
};
#endif
