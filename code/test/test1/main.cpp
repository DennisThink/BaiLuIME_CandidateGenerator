#include <iostream>
#include <string>
#include <chrono>
#include "CandidateGenerator.h"
#include "SimpleCandidateGenerator.h"
#include "SimpleQuanPinGenerator.h"
int main(int argc, char* argv[])
{
    std::cout << "test1" << std::endl;
    std::wcout.imbue(std::locale("chs"));
    if(0)
    {
        SimpleCandidateGenerator generator;
        {
            std::wstring input = L"ni";
            auto candidates = generator.Generate(input, 5);
            for (auto& item : candidates)
            {
                std::wcout << item << " " << std::endl;
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
    }
    {
        auto start = std::chrono::steady_clock::now();
        CSimpleQuanPinGenerator generator;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;
        std::wcout << L"Read Time: " << ms.count() << L" ms" << std::endl;
        {
            std::wstring input = L"ni";
            auto candidates = generator.Generate(input, 5);
            for (auto& item : candidates)
            {
                std::wcout << item << std::endl;
            }
            std::flush(std::wcout);
        }

        {
            std::wstring input = L"wo";
            auto candidates = generator.Generate(input, 5);
            for (auto& item : candidates)
            {
                std::wcout << item << std::endl;
            }
            std::flush(std::wcout);
        }
        {
            std::wstring input = L"ta";
            auto candidates = generator.Generate(input, 5);
            for (auto& item : candidates)
            {
                std::wcout << item << std::endl;
            }
            std::flush(std::wcout);
        }

    }
    return 0;
}