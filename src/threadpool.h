#ifndef threadpool_h
#define threadpool_h

#include <vector>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(size_t threadPoolSize) : m_threadPoolSize(threadPoolSize)
    {}

    virtual ~ThreadPool() = default;

    ThreadPool(ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    bool Start();
    void Stop();

protected:
    virtual void Executor() = 0;

    size_t m_threadPoolSize{0};

private:
    std::vector<std::thread> m_threads;
    bool m_started{false};
};

#endif // threadpool_h