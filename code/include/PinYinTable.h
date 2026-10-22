#ifndef _BAI_LU_IME_PIN_YIN_TABLE_H_
#define _BAI_LU_IME_PIN_YIN_TABLE_H_
#include <string>
#include <vector>
using CountVector = std::vector<int>;
class CPinYinTable
{
public:
	//
	static bool IsInitial(const std::string& strPinYin);

	static bool IsFinal(const std::string& strPinYin);
	
	static int ChineseMaxCount(const std::string& strPinYin);
	static int ChineseMinCount(const std::string& strPinYin);
};
#endif