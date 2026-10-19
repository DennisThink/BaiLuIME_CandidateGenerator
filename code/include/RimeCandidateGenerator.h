#ifndef _BAILU_IME_RIME_CANDIDATE_GENERATOR_H_
#define _BAILU_IME_RIME_CANDIDATE_GENERATOR_H_
#include "CandidateGenerator.h"
#include "IDictionaryInterface.h"
#include <memory>
#include <map>
using CandidateValue = std::vector<std::string>;
using CandidateKey = std::string;
using CandidateMap = std::map < CandidateKey, CandidateValue>;
class CRimeCandiateGenerator :public CandidateGenerator
{
public:
    CRimeCandiateGenerator(const std::string& dictFilePath);
    CRimeCandiateGenerator();
    virtual ~CRimeCandiateGenerator();
    virtual std::vector<std::string> Generate(
        const std::string& input,
        std::size_t maxCandidates)override;
	virtual bool IsDataReady() const override;
private:
	void InitDictionary();
	void UninitDictionary();
	bool ReadDataFromFile(const std::string strFileName);
    std::unique_ptr<IDictionary> m_pDictionary;
    std::vector<CandidateMap> m_CandidateMaps;
    std::map<std::string, std::vector<std::string>> m_simpleDirectory;
};
#endif