#include "SimpleCandidateGenerator.h"

CandidateArray SimpleCandidateGenerator::Generate(
    const std::string& input,
    std::size_t maxCandidates)
{
    CandidateArray candidates;

    if (maxCandidates == 0)
    {
        return candidates;
    }

    /*if (input == "ni")
    {
        candidates.push_back("你");
        candidates.push_back("呢");
        candidates.push_back("泥");
        candidates.push_back("拟");
        candidates.push_back("逆");
    }
    else if (input == "hao")
    {
        candidates.push_back("好");
        candidates.push_back("号");
        candidates.push_back("浩");
    }
    else if (input == "he")
    {
        candidates.push_back("he");
        candidates.push_back("her");
        candidates.push_back("hello");
        candidates.push_back("help");
        candidates.push_back("here");
    }

    if (candidates.size() > maxCandidates)
    {
        candidates.resize(maxCandidates);
    }*/

    return candidates;
}