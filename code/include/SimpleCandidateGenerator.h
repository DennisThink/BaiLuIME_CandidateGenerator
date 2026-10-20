#pragma once

#include "CandidateGenerator.h"

class SimpleCandidateGenerator : public CandidateGenerator
{
public:
    virtual CandidateArray Generate(
        const std::string& input,
        std::size_t maxCandidates)override;
    virtual bool IsDataReady() const override
    {
        return true;
	}
};