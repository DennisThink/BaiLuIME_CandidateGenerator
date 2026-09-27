#ifndef _BAILU_IME_RIME_CANDIDATE_GENERATOR_H_
#define _BAILU_IME_RIME_CANDIDATE_GENERATOR_H_
#include "CandidateGenerator.h"
#include <map>
class CRimeCandiateGenerator :public CandidateGenerator
{
public:
    CRimeCandiateGenerator();
    virtual ~CRimeCandiateGenerator();
    virtual std::vector<std::wstring> Generate(
        const std::wstring& input,
        std::size_t maxCandidates)override;
private:
    std::map<std::wstring, std::vector<std::wstring>> m_simpleDirectory;
};
#endif