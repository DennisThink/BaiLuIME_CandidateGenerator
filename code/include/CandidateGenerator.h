#pragma once

#include <string>
#include <vector>
struct CandidateElem
{
    std::string m_strChinese;
    int m_nWeight;
public:
    CandidateElem() = default;
};
using CandidateArray = std::vector<CandidateElem>;
struct CandidateResult
{
    std::string m_strUserInput;
    CandidateArray m_candidates;
};
class CandidateGenerator
{
public:
    virtual ~CandidateGenerator() = default;
	virtual bool IsDataReady() const = 0;
    virtual CandidateResult Generate(
        const std::string& input,
        std::size_t maxCandidates) = 0;
};