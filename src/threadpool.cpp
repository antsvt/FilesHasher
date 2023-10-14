#include "threadpool.h"

#include "utils.h"

bool ThreadPool::Start()
{
    if (m_started)
    {
        return false;
    }
    m_started = true;

    for (size_t i = 0; i < m_threadPoolSize; i++)
    {
        m_threads.push_back(std::thread([&](){
            try
            {
                this->Executor();
            }
            catch(const std::exception& e)
            {
                Fatal(e.what());
            }
        }));
    }

    return true;
}

void ThreadPool::Stop()
{
    for (std::thread & th : m_threads)
    {
        if (th.joinable())
            th.join();
    }
}