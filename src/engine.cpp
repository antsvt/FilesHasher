#include "engine.h"

#include <chrono>

void Engine::AddBlock(std::shared_ptr<MemoryBuffer> buffer, size_t offset, size_t blockNumber, size_t dataLength)
{
    Block block{buffer, offset, blockNumber, dataLength};

    std::lock_guard<std::mutex> guard(m_mutex);
    m_blocks.push(block);
}

// multytreading hash calculation
void Engine::Executor()
{
    while(true)
    {
        bool needCalculate = false;
        Block block;
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            if (!m_blocks.empty())
            {
                needCalculate = true;
                block = m_blocks.front();
                m_blocks.pop();
            }
        }

        if (needCalculate)
        {
            m_writer.AddResult(block.blockNumber, 
                m_hashCalc.CalculateHash(block.buffer->Data(block.offset), block.dataLength));
        }
        else if (m_noMoreData)
        {
            break;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}