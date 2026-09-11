#include "SimpleCandidateGenerator.h"

std::vector<std::wstring> SimpleCandidateGenerator::Generate(
    const std::wstring& input,
    std::size_t maxCandidates)
{
    std::vector<std::wstring> candidates;

    if (maxCandidates == 0)
    {
        return candidates;
    }

    if (input == L"ni")
    {
        candidates.push_back(L"你");
        candidates.push_back(L"呢");
        candidates.push_back(L"泥");
        candidates.push_back(L"拟");
        candidates.push_back(L"逆");
    }
    else if (input == L"hao")
    {
        candidates.push_back(L"好");
        candidates.push_back(L"号");
        candidates.push_back(L"浩");
    }
    else if (input == L"he")
    {
        candidates.push_back(L"he");
        candidates.push_back(L"her");
        candidates.push_back(L"hello");
        candidates.push_back(L"help");
        candidates.push_back(L"here");
    }

    if (candidates.size() > maxCandidates)
    {
        candidates.resize(maxCandidates);
    }

    return candidates;
}