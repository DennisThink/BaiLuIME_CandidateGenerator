#pragma once

#include "CandidateGenerator.h"

class SimpleCandidateGenerator : public CandidateGenerator
{
public:
    std::vector<std::wstring> Generate(
        const std::wstring& input,
        std::size_t maxCandidates)override;
};