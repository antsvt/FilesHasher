#ifndef engine_h
#define engine_h

#include <stddef.h>
#include <atomic>
#include <queue>
#include <mutex>

#include "threadpool.h"
#include "hash_calculator.h"
#include "results_writer.h"
#include "config.h"
#include "memory_buffer.h"

class Engine final : public ThreadPool
{
public:
    Engine(ResultsWriter &writer, size_t blockSize, size_t threadPoolSize)
        : ThreadPool(threadPoolSize), m_writer(writer), m_hashCalc(blockSize), m_blockSize(blockSize)
    {}

    ~Engine() = default;

    Engine(Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    void NoMoreData()
    {
        m_noMoreData = true;
    }

    void AddBlock(std::shared_ptr<MemoryBuffer> buffer, size_t offset, size_t blockNumber, size_t dataLength);

private:
    void Executor() override;

private:
    struct Block
    {
        std::shared_ptr<MemoryBuffer> buffer;
        size_t offset;
        size_t blockNumber;
        size_t dataLength;
    };

    std::mutex m_mutex;
    std::queue<Block> m_blocks;

    size_t m_blockSize;

    HashCalculator m_hashCalc;
    ResultsWriter &m_writer;

    bool m_noMoreData{false};
};

#endif // engine_h