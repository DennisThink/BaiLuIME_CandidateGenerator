#pragma once

#include <string>
#include <vector>

class CandidateGenerator
{
public:
    virtual ~CandidateGenerator() = default;
	virtual bool IsDataReady() const = 0;
    virtual std::vector<std::string> Generate(
        const std::string& input,
        std::size_t maxCandidates) = 0;
};