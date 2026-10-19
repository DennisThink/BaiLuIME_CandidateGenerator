#include "LongestCommonSubstringDistance.h"
#include "doctest.h"
TEST_CASE("LongestCommonSubstring") {
	CLongestCommonSubstringDistance distance;
	std::string strFirst = "tianya";
	std::string strSecond = "ty";
	int nValue = distance.GetDistance(strFirst, strSecond);
	CHECK(nValue == 2);
}