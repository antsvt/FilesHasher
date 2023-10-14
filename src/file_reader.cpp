#include "file_reader.h"
#include "memory_buffer.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>

void FileReader::Executor()
{
    size_t threadNum = m_threadsCounter++;

    std::fstream inputFile(m_inputPath.u8string(), std::ios::in | std::ios::binary);
    if (inputFile.is_open() == false)
    {
		Fatal("Cannot open file: " + m_inputPath.u8string());
    }

    // make internal buf size divisible by block size
    size_t buffSize = CachBuffSize - CachBuffSize % m_blockSize;
    size_t blocksInBuf = buffSize / m_blockSize;

    size_t blocksPerThread = m_blocksAmount / m_threadPoolSize;
    if (m_blocksAmount % m_threadPoolSize)
    {
        blocksPerThread += 1;
    }

    size_t currentBlock = blocksPerThread * threadNum;
    size_t endBlock = blocksPerThread * (threadNum + 1);

    std::streampos offset = std::streampos(currentBlock) * m_blockSize;

    inputFile.seekg(offset);
    while(currentBlock < endBlock)
    {
        std::shared_ptr<MemoryBuffer> buffer;
        while(true)
        {
            size_t howManyBuffers = MemoryBuffer::m_buffersCounter++;
            if (MaxAmountOfBuffers >= howManyBuffers)
            {
                MemoryBuffer::m_buffersCounter--;
                buffer = std::make_shared<MemoryBuffer>(buffSize);
                break;
            }

            MemoryBuffer::m_buffersCounter--;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::streampos teilSize = m_fileSize - std::streampos(currentBlock * m_blockSize);

        int dataInBuffer = (teilSize > buffSize)?std::streampos(buffSize):teilSize;
        inputFile.read(buffer->Data(0), dataInBuffer);
        if (inputFile.bad())
        {
            Fatal("Got error during file reading. File: " + m_inputPath.u8string());
        }

        for(int i = 0; (i < buffSize/m_blockSize) && (dataInBuffer > 0); i++)
        {
            int dataInBlockSize = (dataInBuffer > m_blockSize)?m_blockSize: dataInBuffer;

            m_engine.AddBlock(buffer, i*m_blockSize, currentBlock, dataInBlockSize);
            
            currentBlock++;
            dataInBuffer -= dataInBlockSize;
        }
    }

    inputFile.close();
}