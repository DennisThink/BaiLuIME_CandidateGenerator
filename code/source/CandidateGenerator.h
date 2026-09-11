#pragma once

#include <string>
#include <vector>

class CandidateGenerator
{
public:
    virtual ~CandidateGenerator() = default;

    virtual std::vector<std::wstring> Generate(
        const std::wstring& input,
        std::size_t maxCandidates) = 0;
};