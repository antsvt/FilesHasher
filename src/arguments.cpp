#include "arguments.h"

#include <boost/program_options.hpp>

#include <exception>
#include <iostream>
#include "config.h"

namespace po = boost::program_options;
namespace fs = std::filesystem;

bool ParseArguments(int argc, char *argv[],
                    size_t& blockSize,
                    std::wstring& input,
                    std::wstring& output)
{
    try 
    {        
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "help message")
            ("input,i", po::wvalue<std::wstring>(&input), "path to input file")
            ("output,o", po::wvalue<std::wstring>(&output), "path to output file")
            ("blocksize,b", po::value<size_t>(&blockSize)->default_value(DefaultBlockSize), "set block size, should be from 512 to 10485760");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return false;
        }

        std::cout << "Block size is set to " << blockSize << std::endl;

        if (vm.count("input") == 0)
        {
            std::cerr << "Argument input is requered, please check help" << std::endl;
            return false;
        }

        if (vm.count("output") == 0)
        {
            std::cerr << "Argument output is requered, please check help" << std::endl;
            return false;
        }
    }
    catch(const po::error& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    catch(const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
        return false;
    }

    return true;
}

bool ValidateArguments(size_t blockSize,
                       const fs::path& input,
                       const fs::path& output)
{
    if (blockSize < MinBlockSize || blockSize > MaxBlockSize)
    {
        std::cerr << "Block size should be from " << MinBlockSize << " to " << MaxBlockSize 
            << ". Block size from arguments is " << blockSize << std::endl;
        return false;
    }

    try
    {
        if (!fs::exists(input))
        {
            std::cerr << "Inpput file " << input.string() << " doesn't exist." << std::endl;
            return false;
        }

        if (fs::exists(output))
        {
            fs::remove(output);
        }
    }
    catch(const fs::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    catch(const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
        return false;
    }

    return true;
}