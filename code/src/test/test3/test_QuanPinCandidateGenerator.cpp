#include "SimpleQuanPinGenerator.h"
#include "doctest.h"
TEST_CASE("LoadFromFile/SimpleQuanPinGenerator") {
	std::string strFolder = "F:\\Github\\BaiLuIME_TestDictionary\\";
	{
		std::string strFilePath = strFolder + "SampleIMESimplifiedQuanPin_SMALL.txt";
		CSimpleQuanPinGenerator generator(strFilePath);
		CHECK(generator.IsDataReady()==true);
	}
}

TEST_CASE("SimpleGenerate/SimpleQuanPinGenerator") {
	std::string strFolder = "F:\\Github\\BaiLuIME_TestDictionary\\";
	{
		std::string strFilePath = strFolder + "SampleIMESimplifiedQuanPin_SMALL.txt";
		CSimpleQuanPinGenerator generator(strFilePath);
		CHECK(generator.IsDataReady() == true);
		std::string strUserInput = "ni";
		std::vector<std::string> result = generator.Generate(strUserInput, 5);
		CHECK(result.size() >= 5);
	}
}