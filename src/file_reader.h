#ifndef file_reader_h
#define file_reader_h

#include <filesystem>
#include <atomic>

#include "engine.h"
#include "threadpool.h"

#include "config.h"

class FileReader final : public ThreadPool
{
public:
    FileReader(const std::filesystem::path &inputPath, 
               std::streampos fileSize, 
               size_t blockSize, 
               size_t blocksAmount, 
               Engine &engine, 
               size_t threadPoolSize)
        : ThreadPool(threadPoolSize), 
          m_inputPath(inputPath), 
          m_fileSize(fileSize), 
          m_blockSize(blockSize), 
          m_blocksAmount(blocksAmount), 
          m_engine(engine)
    {}

    ~FileReader() = default;

    FileReader(FileReader&) = delete;
    FileReader(FileReader&&) = delete;
    FileReader& operator=(FileReader&) = delete;
    FileReader& operator=(FileReader&&) = delete;

private:
    void Executor() override;

private:
    Engine &m_engine;
    std::atomic<size_t> m_threadsCounter{0};

    std::filesystem::path m_inputPath;

    size_t m_blockSize;
    size_t m_blocksAmount;
    std::streampos m_fileSize;
};

#endif // file_reader_h