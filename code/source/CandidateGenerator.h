#pragma once

#include <string>
#include <vector>

class CandidateGenerator
{
public:
    virtual ~CandidateGenerator() = default;

    virtual std::vector<std::string> Generate(
        const std::string& input,
        std::size_t maxCandidates) = 0;
};