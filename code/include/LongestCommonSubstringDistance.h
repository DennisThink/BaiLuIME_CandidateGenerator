#ifndef _BAI_LU_LONGEST_COMMON_SUBSTRING_DISTANCE_H_
#define _BAI_LU_LONGEST_COMMON_SUBSTRING_DISTANCE_H_
#include "IDistanceCalculator.h"
class CLongestCommonSubstringDistance :public IDistanceCalculator
{
public:
	virtual int GetDistance(const std::string& strFirst, const std::string& strSecond) override;
	virtual int GetDistance(const std::wstring& strFirst, const std::wstring& strSecond) override;
private:
	static constexpr int LENGTH = 32;
	int m_valueMatrix[LENGTH][LENGTH];
	void ResetMatrix();
};
#endif