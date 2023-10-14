#ifndef hash_calculator_h
#define hash_calculator_h

#include <string>
#include <vector>
#include <cstring>

class HashCalculator final
{
public:
    explicit HashCalculator(size_t blockSize) : m_blockSize(blockSize)
    {}

    ~HashCalculator() = default;
    
    HashCalculator(HashCalculator&) = delete;
    HashCalculator(HashCalculator&&) = delete;
    HashCalculator& operator=(HashCalculator&) = delete;
    HashCalculator& operator=(HashCalculator&&) = delete;

    std::string CalculateHash(void const* buffer, size_t byteCount);

private:
    size_t m_blockSize{0};
};

#endif // hash_calculator_h