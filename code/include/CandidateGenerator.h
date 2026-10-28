#pragma once

#include <string>
#include <vector>
#include "CommonStruct.h"
struct CandidateResult
{
    std::string m_strUserInput;
    CandidateVector m_candidates;
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