#include "hash_calculator.h"

#include <boost/uuid/detail/md5.hpp>

#include <sstream>
#include <iomanip>

using md5 = boost::uuids::detail::md5;

std::string HashCalculator::CalculateHash(void const* buffer, size_t byteCount)
{
    const int Md5BlockSize = 64;
    md5 md5Calc;

    // callculate hash
    if (byteCount >= m_blockSize)
    {
        md5Calc.process_bytes(buffer, m_blockSize);
    }
    else
    {
        // hash calculated by blocks with size 64 bytes. 
        // if buffer is less than block size, than need
        // to fill gap by zeros. 

        unsigned char baseBlock[Md5BlockSize] = {0};
        size_t tailLength = byteCount % Md5BlockSize;
        size_t blockedPart = byteCount - tailLength;

        const unsigned char* pointer = reinterpret_cast<const unsigned char*>(buffer);
        memcpy(baseBlock, &pointer[blockedPart], tailLength);

        // calculate main part
        if (blockedPart)
        {
            md5Calc.process_bytes(buffer, blockedPart);
        }

        // calculate last part
        bool needClear = true;
        byteCount = m_blockSize - blockedPart;
        while (byteCount > Md5BlockSize)
        {
            md5Calc.process_bytes(baseBlock, Md5BlockSize);
            if (needClear)
            {
                memset(baseBlock, 0, tailLength);
                needClear = false;
            }
            byteCount -= Md5BlockSize;
        }

        // finish last part
        if (byteCount)
        {
            md5Calc.process_bytes(baseBlock, byteCount);
        }
    }

    // get hash
    md5::digest_type digest;
    md5Calc.get_digest(digest);

    std::vector<char> str(sizeof(digest)*2 + 1);
    sprintf(str.data(), "%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3]);
    std::string result(str.data());
    return result;
}