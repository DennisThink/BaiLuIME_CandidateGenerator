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
#include <chrono>
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
		/*generators.push_back(std::make_unique<SimpleCandidateGenerator>());
        generators.push_back(std::make_unique<CSimpleQuanPinGenerator>());

        {
			std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\SampleIMESimplifiedQuanPin_SMALL.txt";
            generators.push_back(std::make_unique<CSimpleQuanPinGenerator>(dictFilePath));
        }

        */
        {
            std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\8105.dict.txt";
			generators.push_back(std::make_unique<CRimeCandiateGenerator>(dictFilePath));
        }

        { 
            std::string dictFilePath = "F:\\Github\\BaiLuIME_TestDictionary\\base.dict.txt";
            generators.push_back(std::make_unique<CRimeCandiateGenerator>(dictFilePath));
        }
		//generators.push_back(std::make_unique<CLocalTcpGenerator>());
    }
	//std::vector<std::string> testInputs = { "ni", "hao", "he", "wo", "ta"};
    std::vector<std::string> testInputs = { "tianya", "taiyang", "ty" };
    {
        for(auto& generatorItem:generators)
        {
            for (auto& input : testInputs)
            {
                for (int count = 10; count < 100; count += 10)
                {
                    auto startTime = std::chrono::steady_clock::now();
                    auto candidates = generatorItem->Generate(input, count);
                    auto endTime = std::chrono::steady_clock::now();
                    double nsCount = std::chrono::duration<double, std::nano>(endTime - startTime).count();
                    std::cout << "Input: " << input <<" Count: "<< count << "  NeedTime: " << nsCount << " ns   " << nsCount / 1000 << " us " << nsCount / 1000000 << "ms" << std::endl;
                    std::cout << "Candidates: ";
                    for (auto& item : candidates)
                    {
                        std::cout << item.m_nWeight << "  " << item.m_strChinese << " ";
                    }
                    std::cout << std::endl;
                }
            }
		}
    }
   
    {
        int value = 0;
        std::cin >> value;
    }
    return 0;
}