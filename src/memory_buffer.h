#ifndef memory_buffer_h
#define memory_buffer_h

#include <atomic>
#include <stddef.h>

class MemoryBuffer final
{
public:
    explicit MemoryBuffer(size_t bufSize) : m_bufSize(bufSize)
    {
        m_buffersCounter++;
        m_buffer = new char[bufSize];
    }

    ~MemoryBuffer()
    {
        delete[] m_buffer;
        m_buffersCounter--;
    }

    MemoryBuffer(MemoryBuffer&) = delete;
    MemoryBuffer(MemoryBuffer&&) = delete;
    MemoryBuffer& operator=(MemoryBuffer&) = delete;
    MemoryBuffer& operator=(MemoryBuffer&&) = delete;

    char* Data(size_t offset) const
    { 
        return &m_buffer[offset];
    }

public:
    static std::atomic<size_t> m_buffersCounter;

private:
    char* m_buffer{nullptr};
    size_t m_bufSize{0};
};

#endif // memory_buffer_h