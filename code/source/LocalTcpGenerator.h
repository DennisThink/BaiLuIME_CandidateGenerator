#ifndef _LOCAL_TCP_GENERATOR_H_
#define _LOCAL_TCP_GENERATOR_H_
#include "CandidateGenerator.h"
#include <map>
#include <string>
class CLocalTcpGenerator :public CandidateGenerator
{
public:
    CLocalTcpGenerator();
	virtual ~CLocalTcpGenerator();
public:
    virtual std::vector<std::wstring> Generate(
        const std::wstring& input,
        std::size_t maxCandidates) override;
};
#endif