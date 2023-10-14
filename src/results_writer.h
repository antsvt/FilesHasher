#ifndef results_writer_h
#define results_writer_h

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <filesystem> 
#include <unordered_map>

#include "threadpool.h"


class ResultsWriter final : public ThreadPool
{
public:
    ResultsWriter(const std::filesystem::path &outputPath, size_t maxBlockNummer)
        : ThreadPool(1),  m_outputPath(outputPath), m_maxBlockNummer(maxBlockNummer)
    {}

    ~ResultsWriter() = default;

    ResultsWriter(ResultsWriter&) = delete;
    ResultsWriter(ResultsWriter&&) = delete;
    ResultsWriter& operator=(ResultsWriter&) = delete;
    ResultsWriter& operator=(ResultsWriter&&) = delete;

    void AddResult(size_t blockNumber, const std::string &hash);

private:
    void Executor() override;

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_readyFlag = false;

    std::unordered_map<size_t, std::string> m_results;

    std::filesystem::path m_outputPath;

    size_t m_maxBlockNummer{0};
    size_t m_currentBlockNummer{0};
};

#endif // results_writer_h