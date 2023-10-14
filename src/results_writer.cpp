#include "results_writer.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <chrono>

void ResultsWriter::Executor()
{
    std::ofstream outputFile(m_outputPath.u8string());
    if (outputFile.is_open() == false)
    {
		Fatal("Cannot open result file: " + m_outputPath.u8string());
    }

    while (m_currentBlockNummer < m_maxBlockNummer)
    {
        std::string hash;
        // check if current block is here
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            auto search = m_results.find(m_currentBlockNummer);
            if (search != m_results.end()) 
            {
                m_currentBlockNummer++;
                hash = search->second;
                m_results.erase(search);
            }
        }

        if (hash.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else
        {
            hash += "\n";
            outputFile << hash;
        }
    }

    outputFile.close();
}

void ResultsWriter::AddResult(size_t blockNumber, const std::string &hash)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_results.insert({blockNumber, hash});
}
