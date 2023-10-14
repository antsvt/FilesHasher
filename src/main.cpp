#include "arguments.h"
#include "file_reader.h"
#include "engine.h"
#include "results_writer.h"
#include "utils.h"

#include "config.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    size_t blockSize = DefaultBlockSize;
    std::wstring input;
    std::wstring output;

    if (!ParseArguments(argc, argv, blockSize, input, output))
    {
        return -1;
    }

    if (!ValidateArguments(blockSize, input, output))
    {
        return -1;
    }

    // convert path from unicode to native format
    std::filesystem::path inputPath(input);
    std::fstream inputFile(inputPath.u8string(), std::ios::in | std::ios::binary | std::ios::ate);
    if (inputFile.is_open() == false)
    {
        Fatal("Cannot open file: " + inputPath.u8string());
    }
    std::streampos fileSize = inputFile.tellg();
    inputFile.close();

    if (fileSize == 0)
    {
        Fatal("File is empty. File: " + inputPath.u8string());
    }

    size_t blocksAmount = fileSize/blockSize;
    if (fileSize%blockSize)
    {
        blocksAmount++;
    }

    ResultsWriter writer(output, blocksAmount);
    Engine engine(writer, blockSize, 2 * std::thread::hardware_concurrency());
    FileReader fileReader(input, fileSize, blockSize, blocksAmount, engine, std::thread::hardware_concurrency());
    
    std::cout << "Cores in the system: " << std::thread::hardware_concurrency() << std::endl;

    writer.Start();
    engine.Start();
    fileReader.Start();

    // wait here for the execution
    fileReader.Stop();
    std::cout << "All data is read " << std::endl;

    engine.NoMoreData();
    engine.Stop();
    std::cout << "All hashes are calculated " << std::endl;

    writer.Stop();

    return 0;
}
