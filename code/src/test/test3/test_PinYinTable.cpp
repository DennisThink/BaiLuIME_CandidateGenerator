#include "PinYinTable.h"
#include "doctest.h"
TEST_CASE("InitialCount") {
	{
		std::string strPinYin = "tianya";
		int nMin = CPinYinTable::ChineseMinCount(strPinYin);
		int nMax = CPinYinTable::ChineseMaxCount(strPinYin);
		CHECK_EQ(nMin, 2);
		CHECK_EQ(nMax, 3);
		
	}

	{
		std::string strPinYin = "taiyang";
		int nMin = CPinYinTable::ChineseMinCount(strPinYin);
		int nMax = CPinYinTable::ChineseMaxCount(strPinYin);
		CHECK_EQ(nMin, 2);
		CHECK_EQ(nMax, 4);
	}

	{
		std::string strPinYin = "ty";
		int nMin = CPinYinTable::ChineseMinCount(strPinYin);
		int nMax = CPinYinTable::ChineseMaxCount(strPinYin);
		CHECK_EQ(nMin, 2);
		CHECK_EQ(nMax, 2);
	}
}