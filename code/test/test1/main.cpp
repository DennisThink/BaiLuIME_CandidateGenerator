#include <iostream>
#include <string>
#include "CandidateGenerator.h"
#include "SimpleCandidateGenerator.h"
int main(int argc,char * argv[])
{
    std::cout<<"test1"<<std::endl;
	std::wcout.imbue(std::locale("chs"));
    SimpleCandidateGenerator generator;
    {
        std::wstring input = L"ni";
		auto candidates = generator.Generate(input,5);
        for(auto& item:candidates)
        {
            std::wcout<<item<<" "<<std::endl;
		}
		std::flush(std::wcout);
    }
    {
        std::wstring input = L"ni";
        auto candidates = generator.Generate(input, 5);
        for (auto& item : candidates)
        {
            std::wcout << item << std::endl;
        }
        std::flush(std::wcout);
    }
    return 0;
}