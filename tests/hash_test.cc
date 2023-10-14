#include <gtest/gtest.h>
#include <hash_calculator.h>

TEST(HashCalculatorTest, FullBlockCorrectnessTest) 
{
    std::string message{"message"};
    HashCalculator hashCalc(message.size());

    EXPECT_EQ(hashCalc.CalculateHash(message.c_str(), message.size()), 
              "78e731027d8fd50ed642340b7c9a63b3");
}

TEST(HashCalculatorTest, PartialBlocksTest) 
{
    const int blockSize[] = 
    {
        64,     // exact md5 block size
        63,     // less than one md5 block
        64,     // more than one md5 block
        127,    // less than two md5 blocks
        128,    // exact two md5 blocks
        129     // more than two md5 blocks
    };

    for (int i = 0; i < sizeof(blockSize)/sizeof(blockSize[0]); i++)
    {
        HashCalculator hashCalc(blockSize[i]);
        std::string message{"message"};
        std::vector<char> buffer(blockSize[i]);
        memset(buffer.data(), 0, blockSize[i]);
        memcpy(buffer.data(), message.c_str(), message.size());

        EXPECT_EQ(hashCalc.CalculateHash(message.c_str(), message.size()), 
                hashCalc.CalculateHash(buffer.data(), blockSize[i]));
    }
}
