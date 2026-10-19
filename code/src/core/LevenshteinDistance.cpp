#include "LevenshteinDistance.h"
int CLevenshteinDistance::GetDistance(const std::string& strFirst, const std::string& strSecond)
{
	std::size_t First_Length = strFirst.length();
	std::size_t Second_Length = strSecond.length();
	if (First_Length < LENGTH && Second_Length < LENGTH)
	{
		ResetMatrix();
		for (std::size_t i = 1; i < First_Length;i++)
		{
			for (std::size_t j = 1; j < Second_Length; j++)
			{
				int cost = (strFirst[i] == strSecond[j]) ? 0 : 1;
				m_valueMatrix[i][j] = std::min(
					{
						m_valueMatrix[i-1][j]+1,
						m_valueMatrix[i][j-1]+1,
						m_valueMatrix[i-1][j-1] + cost
					});
			}
		}
		return m_valueMatrix[First_Length - 1][Second_Length - 1];
	}
	else
	{

	}
	
	return -1;
}


int CLevenshteinDistance::GetDistance(const std::wstring& strFirst, const std::wstring& strSecond)
{
	return -1;
}

void CLevenshteinDistance::ResetMatrix()
{
	for (int i = 0; i < LENGTH; i++)
	{
		m_valueMatrix[i][0] = i;
	}

	for (int j = 0; j < LENGTH; j++)
	{
		m_valueMatrix[0][j] = j;
	}

	for (int i = 1; i < LENGTH; i++)
	{
		for (int j = 1; j < LENGTH;j++)
		{
			m_valueMatrix[i][j] = 0;
		}
	}
}