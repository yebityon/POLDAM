#pragma once
#include <iostream>
#include <chrono>

#include <Util/include/src/poldamUtil.h>
#include <Util/include/src/poldamConfig.h>
#include <MetafileParser/include/factory.h>

void printHelp()
{
}

int main(int argc, char *argv[])
{
    POLDAM::poldamConfig config = {};

    if (argc <= 1)
    {
        std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "You need to disignate input directory." << std::endl;
    }

    for (unsigned int i = 1; i < argc; ++i)
    {

        const std::string arg = argv[i];
        if (arg == "-d" or arg == "--outputDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Gieven\n";
                printHelp();
                exit(1);
            }
            const std::string outputDir = argv[i + 1];
            config.outputDir = outputDir;
            ++i;
        }
        else if (arg == "-s" or arg == "--inputDir")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Given\n";
                printHelp();
                exit(1);
            }
            const std::string inputDir = argv[i + 1];
            config.inputDir = inputDir;
            ++i;
        }
        else if (arg == "-t" or arg == "--targetMethod")
        {
            if (i + 1 >= argc)
            {
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No targetMethod is Given\n";
                printHelp();
                exit(1);
            }
            const std::string targetMethod = argv[i + 1];
            config.targetMethodName = targetMethod;
            ++i;
        }
        else if (arg == "--fastIO")
        {
            std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Use fast IO\n"
                      << std::endl;
            std::cin.tie(nullptr);
            std::ios_base::sync_with_stdio(false);
            config.useFastIO = true;
        }
        else
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Unknown Option\n";
            printHelp();
            exit(1);
        }
    }

    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "PODLAM IS WORKING\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "outputDir: {" << config.outputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "inputDir: {" << config.inputDir << "}\n";
    std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetMethod: {" << config.targetMethodName << "}\n";

    // it should be static
    POLDAM::metafileFactory factory(config.inputDir);

    auto dataids = factory.createInstance<POLDAM::dataidsParser>(config.inputDir);
}
