#include <gtest/gtest.h>
#include <arguments.h>

TEST(ParseArgumentsTest, Normal) 
{
    size_t blockSize;
    std::wstring input;
    std::wstring output;

    char *argv[] = {"hasher_test", "-b", "512", "-i", "source", "-o", "result"};

    EXPECT_TRUE(ParseArguments(sizeof(argv)/sizeof(argv[0]), argv, blockSize, input, output));
    EXPECT_EQ(input, L"source");
    EXPECT_EQ(output, L"result");
    EXPECT_EQ(blockSize, 512);
}

TEST(ParseArgumentsTest, NoInput) 
{
    size_t blockSize;
    std::wstring input;
    std::wstring output;

    char *argv[] = {"hasher_test", "-b", "512", "-o", "result"};

    EXPECT_FALSE(ParseArguments(sizeof(argv)/sizeof(argv[0]), argv, blockSize, input, output));
}

TEST(ParseArgumentsTest, NoOutput) 
{
    size_t blockSize;
    std::wstring input;
    std::wstring output;

    char *argv[] = {"hasher_test", "-b", "512", "-i", "source"};

    EXPECT_FALSE(ParseArguments(sizeof(argv)/sizeof(argv[0]), argv, blockSize, input, output));
}

TEST(ParseArgumentsTest, DefaultBlockSize) 
{
    size_t blockSize = 0;
    std::wstring input;
    std::wstring output;

    char *argv[] = {"hasher_test", "-i", "source", "-o", "result"};

    EXPECT_TRUE(ParseArguments(sizeof(argv)/sizeof(argv[0]), argv, blockSize, input, output));
    EXPECT_EQ(blockSize, 1024*1024);
}

TEST(ParseArgumentsTest, ValidateOkay)
{
    size_t blockSize = 512;
    std::wstring input = L"././hasher_test";
#ifdef _WINDOWS
    input += L".exe";
#endif // _WINDOWS
    std::wstring output = L"./hasher_output";

    EXPECT_TRUE(ValidateArguments(blockSize, input, output));
}

TEST(ParseArgumentsTest, BlockSizeTooSmall)
{
    size_t blockSize = 511;
    std::wstring input = L"././hasher_test";
#ifdef _WINDOWS
    input += L".exe";
#endif // _WINDOWS
    std::wstring output = L"./hasher_output";

    EXPECT_FALSE(ValidateArguments(blockSize, input, output));
}

TEST(ParseArgumentsTest, BlockSizeTooBig)
{
    size_t blockSize = 10485761;
    std::wstring input = L"././hasher_test";
#ifdef _WINDOWS
    input += L".exe";
#endif // _WINDOWS
    std::wstring output = L"./hasher_output";

    EXPECT_FALSE(ValidateArguments(blockSize, input, output));
}