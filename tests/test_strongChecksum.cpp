#include <list>
#include <gtest/gtest.h>

#include "strongchecksum.h"

// See https://en.wikipedia.org/wiki/MD4
TEST(StrongChecksum, Md4TestVectors)
{
    std::list<std::pair<std::string, std::string>> data {
        {"The quick brown fox jumps over the lazy dog", "1bee69a46ba811185c194762abaeae90"},
        {"The quick brown fox jumps over the lazy cog", "b86e130ce7028da59e672d56ad0113df"},
        {"", "31d6cfe0d16ae931b73c59d7e0c089c0"},
        {"a", "bde52cb31de33e46245e05fbdbd6fb24"},
        {"abc", "a448017aaf21d8525fc10ae87aa6729d"},
        {"message digest", "d9130a8164549fe818874806e1c7014b"},
        {"abcdefghijklmnopqrstuvwxyz", "d79e1c308aa5bbcdeea8ed63df412da9"},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "043f8582f241db351ce627e153e7f0e4"},
        {"12345678901234567890123456789012345678901234567890123456789012345678901234567890", "e33b4ddc9c38f2199c3e7b164fcc0536"},

    };

    //may use parametrized tests (data-driven) as well, left as is for simplicity
    for(const auto& input_expectation_pair : data)
    {
        const auto input_string = input_expectation_pair.first;
        const auto expected_string = input_expectation_pair.second;
        const std::vector<unsigned char> input{input_string.cbegin(), input_string.cend()};

        const auto strong_signature = StrongChecksum::calculate(input);
        std::string output;
        for(const int o : strong_signature)
        {
            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << o;
            output += ss.str();
        }
        EXPECT_EQ(output, expected_string);
    }
}
