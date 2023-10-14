#ifndef arguments_h
#define arguments_h

#include <string>
#include <cstddef>
#include <filesystem> 

bool ParseArguments(int argc, char *argv[],
                    size_t& blockSize,
                    std::wstring& input,
                    std::wstring& output);

bool ValidateArguments(size_t blockSize,
                       const std::filesystem::path& input,
                       const std::filesystem::path& output);

#endif // arguments_h