#include "RimeCandidateGenerator.h"
#include "doctest.h"
TEST_CASE("LoadFromFile/RimeCandidateGenerator") {
	std::string strFolder = "F:\\Github\\BaiLuIME_TestDictionary\\";
	{
		std::string strFilePath = strFolder + "8105.dict.txt";
		CRimeCandiateGenerator generator(strFilePath);
		CHECK(generator.IsDataReady() == true);
	}
	{
		std::string strFilePath = strFolder + "base.dict.txt";
		CRimeCandiateGenerator generator(strFilePath);
		CHECK(generator.IsDataReady() == true);
	}
}

TEST_CASE("SimpleGenerate/RimeCandidateGenerator") {
	std::string strFolder = "F:\\Github\\BaiLuIME_TestDictionary\\";
	{
		std::string strFilePath = strFolder + "8105.dict.txt";
		CRimeCandiateGenerator generator(strFilePath);
		CHECK(generator.IsDataReady() == true);
		std::string strUserInput = "ni";
		auto result = generator.Generate(strUserInput, 5);
		CHECK(result.m_candidates.size() >= 5);
	}
	{
		std::string strFilePath = strFolder + "base.dict.txt";
		CRimeCandiateGenerator generator(strFilePath);
		CHECK(generator.IsDataReady() == true);
		std::string strUserInput = "nihao";
		auto result = generator.Generate(strUserInput, 5);
		CHECK(result.m_candidates.size() >= 2);
	}
}