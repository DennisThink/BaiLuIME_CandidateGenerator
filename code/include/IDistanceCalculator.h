#ifndef _BAILU_IME_DISTANCE_CALCULATOR_H_
#define _BAILU_IME_DISTANCE_CALCULATOR_H_
#include <string>
class IDistanceCalculator
{
public:
virtual int GetDistance(const std::string& strFirst,const std::string& strSecond)=0;
virtual int GetDistance(const std::wstring& strFirst,const std::wstring& strSecond)=0;
};
#endif