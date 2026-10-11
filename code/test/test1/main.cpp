#include <iostream>
#include <string>
#include <chrono>
#include "CandidateGenerator.h"
#include "SimpleCandidateGenerator.h"
#include "SimpleQuanPinGenerator.h"
#include "RimeIceDictionary.h"
#include "RimeCandidateGenerator.h"
#include "LocalTcpGenerator.h"
#include <windows.h>
#include <memory>
#include <vector>
using CandidateGeneratorPtr = std::unique_ptr<CandidateGenerator>;
int main(int argc, char* argv[])
{
    std::cout << "test1" << std::endl;
#ifdef _WIN32
    ::SetConsoleOutputCP(CP_UTF8);   // 输出用 UTF-8
    ::SetConsoleCP(CP_UTF8);         // 输入用 UTF-8
#endif
    //std::wcout.imbue(std::locale("chs"));
	std::vector<CandidateGeneratorPtr> generators;
    {
		generators.push_back(std::make_unique<SimpleCandidateGenerator>());
        generators.push_back(std::make_unique<CSimpleQuanPinGenerator>());

        {
			std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\SampleIMESimplifiedQuanPin_SMALL.txt";
            generators.push_back(std::make_unique<CSimpleQuanPinGenerator>(dictFilePath));
        }

        generators.push_back(std::make_unique<CRimeCandiateGenerator>());
        {
            std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\8105.dict.txt";
			generators.push_back(std::make_unique<CSimpleQuanPinGenerator>(dictFilePath));
        }

        { 
            std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\base.dict.txt";
            generators.push_back(std::make_unique<CSimpleQuanPinGenerator>(dictFilePath));   
        }
		generators.push_back(std::make_unique<CLocalTcpGenerator>());
    }
	std::vector<std::string> testInputs = { "ni", "hao", "he", "wo", "ta" };

    {
        for(auto& generatorItem:generators)
        {
            for (auto& input : testInputs)
            {
                auto candidates = generatorItem->Generate(input, 5);
                std::cout << "Input: " << input << std::endl;
                std::cout << "Candidates: ";
                for (auto& item : candidates)
                {
                    std::cout << item << " ";
                }
                std::cout << std::endl;
            }
		}
    }
   
    {
        int value = 0;
        std::cin >> value;
    }
    return 0;
}