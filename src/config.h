#ifndef config_h
#define config_h

constexpr int MinBlockSize = 512;
constexpr int DefaultBlockSize = 1024 * 1024;
constexpr int MaxBlockSize = 10 * 1024 * 1024;

constexpr int CachBuffSize = 10 * 1024 * 1024;
constexpr int MaxAmountOfBuffers = 15;

#endif // config_h
