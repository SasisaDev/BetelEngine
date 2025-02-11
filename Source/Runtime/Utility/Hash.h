#pragma once

#include <string>
#include <cstdint>

struct Hash {
    static uint64_t Hash64(const std::string& str) {
        const int p = 53;
        const int m = 1e9 + 9;
        uint64_t hash_value = 0;
        uint64_t p_pow = 1;
        for (char c : str) {
            hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hash_value;
    }
};