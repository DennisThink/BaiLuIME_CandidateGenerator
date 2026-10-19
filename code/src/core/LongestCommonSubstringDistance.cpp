#include "LongestCommonSubstringDistance.h"

int CLongestCommonSubstringDistance::GetDistance(const std::string& strFirst, const std::string& strSecond)
{
	ResetMatrix();
	std::size_t FIRST_LEN = strFirst.length();
	std::size_t SECOND_LEN = strSecond.length();
	if (FIRST_LEN < LENGTH && SECOND_LEN < LENGTH)
	{
		for (std::size_t i = 1; i <= FIRST_LEN; i++)
		{
			for (std::size_t j = 1; j <= SECOND_LEN; j++)
			{
				//Last charactor same 
				if (strFirst[i - 1] == strSecond[j - 1])
				{
					m_valueMatrix[i][j] = m_valueMatrix[i - 1][j - 1] + 1;
				}
				else
				{
					m_valueMatrix[i][j] = std::max(m_valueMatrix[i - 1][j], m_valueMatrix[i][j - 1]);
				}
			}
		}
		int nValue = m_valueMatrix[FIRST_LEN][SECOND_LEN];
		return nValue;
	}
	return -1;
}
int CLongestCommonSubstringDistance::GetDistance(const std::wstring& strFirst, const std::wstring& strSecond)
{
	return -1;
}

void CLongestCommonSubstringDistance::ResetMatrix()
{
	for (std::size_t i = 0; i < LENGTH; i++)
	{
		for (std::size_t j = 0;j < LENGTH;j++)
		{
			m_valueMatrix[i][j] = 0;
		}
	}
}