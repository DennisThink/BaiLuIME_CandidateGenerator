#include "PinYinTable.h"
#include <vector>
#include <iostream>
static std::vector<std::string> G_INITIAL_STRING_ARRY = {
	"b",
	"p",
	"m",
	"f",
	"d",
	"t",
	"n",
	"l", 
	"g",
	"k",
	"h", 
	"j",
	"q", 
	"x", 
	"zh",
	"ch", 
	"sh",
	"r",
	"z",
	"c",
	"s",
	"y"
};

static std::vector<std::string> G_FINAL_STRING_ARRAY
{
	"a",
	"o",
	"e",
	"i",
	"u",
	"¨¹", 
    "i",
	"er",
	"ai",
	"ei",
	"ao",
	"ou",
	"ia",
	"ie",
	"ua",
	"uo",
	"¨¹e",
	"iao",
	"iou",
	"uai",
	"uei",
	"an",
	"en",
	"in",
	"un",
	"¨¹n",
	"ang",
	"eng",
	"ing",
	"ong",
	"ian",
	"uan",
	"¨¹an",
	"iang",
	"uang",
	"iong",
	"ueng"
};

bool CPinYinTable::IsInitial(const std::string& strPinYin)
{
	for (const auto& item : G_INITIAL_STRING_ARRY)
	{
		if (strPinYin == item)
		{
			std::cout <<" Initial: "<< strPinYin << std::endl;
			return true;
		}
	}
	return false;
}

bool CPinYinTable::IsFinal(const std::string& strPinYin)
{
	for (const auto& item : G_FINAL_STRING_ARRAY)
	{
		if (strPinYin == item)
		{
			return true;
		}
	}
	return false;
}

int CPinYinTable::ChineseMaxCount(const std::string& strPinYin)
{
	int nCount = 0;
	std::string strSingle;
	int nMaxCount = 0;
	std::size_t index = 0;
	while (index < strPinYin.length())
	{
		strSingle.clear();
		strSingle += strPinYin[index];
		if (IsInitial(strSingle))
		{
			
			if ((index + 1) < strPinYin.length())
			{
				strSingle += strPinYin[index + 1];
				if (IsInitial(strSingle))
				{
					nMaxCount += 2;
					index += 2;
				}
				else
				{
					strSingle.clear();
					nMaxCount += 1;
					index += 1;
				}
			}
			else
			{
				strSingle.clear();
				nMaxCount += 1;
				index += 1;
			}
		}
		else
		{
			index += 1;
		}
	}

	return nMaxCount;
}


int CPinYinTable::ChineseMinCount(const std::string& strPinYin)
{
	int nMinCount = 0;
	std::size_t index = 0;
	std::size_t LENGTH = strPinYin.length();
	std::string strSingle;
	while (index < LENGTH)
	{
		while(index < LENGTH)
		{
			strSingle += strPinYin[index];
			if (IsInitial(strSingle))
			{
				if ((index + 1) < LENGTH)
				{
					strSingle += strPinYin[index + 1];
					if (IsInitial(strSingle))
					{
						index += 2;
					}
					else
					{
						index += 1;
					}
				}
				else
				{
					index += 1;
				}
				nMinCount++;
				strSingle.clear();
			}
			else
			{
				break;
			}
		}
		strSingle.clear();
		while (index < LENGTH)
		{
			strSingle += strPinYin[index];
			if (IsFinal(strSingle))
			{
				index++;
			}
			else
			{
				strSingle.clear();
				break;
			}
		}
		
	}

	return nMinCount;

}