#ifndef SHA256_UTIL_H
#define SHA256_UTIL_H

#include <string>
#include <cstdint>

class SHA256Util {
public:
    static std::string hash(const std::string& input);

private:
    static const uint32_t K[64];

    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t sigma0(uint32_t x);
    static uint32_t sigma1(uint32_t x);
    static uint32_t Sigma0(uint32_t x);
    static uint32_t Sigma1(uint32_t x);
};

#endif // SHA256_UTIL_H
