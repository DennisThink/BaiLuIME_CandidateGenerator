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
int main(int argc, char* argv[])
{
    std::cout << "test1" << std::endl;
#ifdef _WIN32
    ::SetConsoleOutputCP(CP_UTF8);   // 输出用 UTF-8
    ::SetConsoleCP(CP_UTF8);         // 输入用 UTF-8
#endif
    //std::wcout.imbue(std::locale("chs"));
    if (1)
    {

        {
            SimpleCandidateGenerator generator;
            {
                std::string input = "ni";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << " " << std::endl;
                }
                std::flush(std::wcout);
            }
            {
                std::string input = "wo";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }
            {
                std::string input = "ta";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
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
                std::string input = "ni";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }

            {
                std::string input = "wo";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }
            {
                std::string input = "ta";
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }
        }
        {
            auto start = std::chrono::steady_clock::now();
            CRimeIceDictionary dictionary;
            dictionary.ReadDataFromFile("8105.dict.txt");
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> ms = end - start;
            std::wcout << L"Read Time: " << ms.count() << L" ms" << std::endl;
            dictionary.WriteDataToFile("8105_Copy.dict.txt");
        }
        {
            auto start = std::chrono::steady_clock::now();
            CRimeIceDictionary dictionary;
            dictionary.ReadDataFromFile("base.dict.txt");
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> ms = end - start;
            std::wcout << L"Read Time: " << ms.count() << L" ms" << std::endl;
            dictionary.WriteDataToFile("base_Copy.dict.txt");
        }
        {
            auto start = std::chrono::steady_clock::now();
            CRimeCandiateGenerator generator;
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> ms = end - start;
            std::wcout << L"Read Time: " << ms.count() << L" ms" << std::endl;
            {
                std::string input = "ni";
                std::wcout.clear();
                auto candidates = generator.Generate(input, 5);
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }

            {
                std::string input = "wo";
                auto candidates = generator.Generate(input, 5);
                std::wcout.clear();
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }
            {
                std::string input = "ta";
                auto candidates = generator.Generate(input, 5);
                std::wcout.clear();
                for (auto& item : candidates)
                {
                    std::cout << item << std::endl;
                }
                std::flush(std::wcout);
            }
        }
    }

    //Local Tcp Generator
    if(0)
    {

        auto start = std::chrono::steady_clock::now();
        CLocalTcpGenerator generator;
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;
        std::wcout << L"Read Time: " << ms.count() << L" ms" << std::endl;
        {
            std::string input = "ni";
            std::wcout.clear();
            auto candidates = generator.Generate(input, 5);
            for (auto& item : candidates)
            {
                std::cout << item << std::endl;
            }
            std::flush(std::wcout);
        }

        {
            std::string input = "wo";
            auto candidates = generator.Generate(input, 5);
            std::wcout.clear();
            for (auto& item : candidates)
            {
                std::cout << item << std::endl;
            }
            std::flush(std::wcout);
        }
        {
            std::string input = "ta";
            auto candidates = generator.Generate(input, 5);
            std::cout.clear();
            for (auto& item : candidates)
            {
                std::cout << item << std::endl;
            }
            std::flush(std::cout);
        }
    }
    {
        int value = 0;
        std::cin >> value;
    }
    return 0;
}