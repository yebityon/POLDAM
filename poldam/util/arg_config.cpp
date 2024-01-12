#pragma once 
#include <poldam/util/utility.hpp>
#include <poldam/util/arg_config.hpp>

namespace POLDAM
{
    void printHelp()
    {
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Usage: [command] "
                  << "-o [original_selogger_directory] -t [target_selogger_directory] -m [target_method] -d [diff_file_name] \n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-o, The path to the original directory. This parameter is mandatory and must point to a valid selogger output directory.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-t, The path to the target directory. This directory data  This parameter is mandatory and must point to a valid selogger output directory.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-c, The Merkle tree will be constructed using the class specified by the -c option and -m option as the entry point.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-m, The Merkle tree will be constructed using the method specified by the -m option as the entry point.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-f, Only outputs vertices that match the regular expression specified by the - f option.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "-d, The file name of the diff file\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "--debug, enalbe debug mode.\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "--flow, --param : evaluate the equivalence of method execution using the given hash. \n";
    }

    poldamConfig generateConfig(int argc, char *argv[])
    {
        poldamConfig config = {};

        if (argc <= 1)
        {
            std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "You need to designate input directory." << std::endl;
            return poldamConfig{};
        }

        for (unsigned int i = 1; i < argc; ++i)
        {
            const std::string arg = argv[i];
            if (arg == "-o" or arg == "--originDir")
            {
                if (i + 1 >= argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Gieven\n";
                    printHelp();
                    return poldamConfig{};
                }
                config.originDir = argv[i + 1];
                ++i;
            }
            else if (arg == "-t" or arg == "--targetDir")
            {
                if (i + 1 >= argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No directory is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                config.targetDir = argv[i + 1];
                ++i;
            }
            else if (arg == "-c")
            {
                if (i + 1 >= argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No targetMethod is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                config.entryClassName = argv[i + 1];
                config.hasEntryClassName = true;
                ++i;
            }
            else if (arg == "-m" or arg == "--targetMethod")
            {
                if (i + 1 >= argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No targetMethod is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                config.entryMethodName = argv[i + 1];
                config.hasEntryMethodName = true;
                ++i;
            }
            else if (arg == "-d" or arg == "--difffilename")
            {
                if (i + 1 > argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No DifffileName is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                config.outputFileName = argv[i + 1];
                ++i;
            }
            else if (arg == "-f" or arg == "--filterd_vertex")
            {
                if (i + 1 > argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No Filterd Vertex is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                if(!config.setFilterdRegExp(argv[i + 1]))
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Invalid Filterd Vertex Regexp:";
                    std::cout << argv[i + 1]  << '\n';
                    return poldamConfig{};
                }
                ++i;
            }
            else if(arg == "--compute_hash_target")
            {
                if (i + 1 > argc)
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "No compute_hash_target regexp is Given\n";
                    printHelp();
                    return poldamConfig{};
                }
                if(!config.setFilterdHashRegExp(argv[i + 1]))
                {
                    std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Invalid compute_hash_target Regexp:";
                    std::cout << argv[i + 1]  << '\n';
                    return poldamConfig{};
                };
                ++i;
            }
            else if (arg == "--debug")
            {
                config.isDebugMode = true;
            }
            else if (arg == "--flow")
            {
                config.useFlowHash = true;
            }
            else if (arg == "--param")
            {
                config.useParamHash = true;
            }
            else
            {
                printHelp();
                std::cout << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX << "Unknown Option.\n";
                std::cout << argv[i] << " is not valid option.\n";
                return poldamConfig{};
            }
        }

        // validate config
        if (config.originDir.size() == 0 || config.targetDir.size() == 0)
        {
            // validなパスかどうかもチェックしたい
            std::cout
                << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX
                << "You need to designate input directory."
                << std::endl;
            return poldamConfig{};
        }
        if (config.hasEntryClassName && !config.hasEntryMethodName)
        {
            // -c は指定したけど-mは指定していない
            std::cout
                << POLDAM_UTIL::POLDAM_WARNING_PRINT_SUFFIX
                << "You have specified the class name but not the method name. "
                << std::endl;
            return poldamConfig{};
        }
        if (config.hasFilterdRegex && !config.hasEntryMethodName)
        {
            // -fは指定したけど、-m は指定してない
            std::cout
                << POLDAM_UTIL::POLDAM_WARNING_PRINT_SUFFIX
                << "You have specified the filterd regex but not the method name. "
                << std::endl;
            return poldamConfig{};
        }
        if (config.hasEntryMethodName && !config.hasEntryClassName)
        {
            // -m は指定したけど-cは指定していない
            std::cout
                << POLDAM_UTIL::POLDAM_ERROR_PRINT_SUFFIX
                << "You have specified the method name but not the class name. "
                << std::endl;
            return poldamConfig{};
        }
        return config;
    }

    void printConfig(const poldamConfig &config)
    {
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "originDir: {" << config.originDir << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "targetDir: {" << config.targetDir << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "entryClass: {" << config.entryClassName << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "entryMethod: {" << config.entryMethodName << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "filteredRegexp: {" << config.filterdVertexRegexStr << "}\n";
        if (config.outputFileName.size() == 0)
        {
            std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "diffFileName is Empty, use default value.\n";
        }
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "entryMethod: {" << (config.hasEntryMethodName ? config.entryMethodName : "No entry method") << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "diffFileName: {" << config.outputFileName << "}\n";
        std::cout << POLDAM_UTIL::POLDAM_PRINT_SUFFIX << "Hash Settings:";
        std::cout << "Flow: " << (config.useFlowHash ? "true, " : "false, ") << "Param: " << (config.useParamHash ? "true" : "false") << std::endl;

        return;
    }
} // namespace 
