#pragma once

#include <vector>

/*!
 * \brief The StrongChecksum class implements MD4 hash function
 * See https://en.wikipedia.org/wiki/MD4
 */

class StrongChecksum
{
public:
    static std::vector<unsigned char> calculate(const std::vector<unsigned char>& input);
};
