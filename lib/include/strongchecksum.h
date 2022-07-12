#pragma once

#include <vector>
#include <string>

/*!
 * \brief The StrongChecksum class implements MD4 hash function
 * See https://en.wikipedia.org/wiki/MD4
 */

class StrongChecksum
{
public:
    static std::string calculate(const std::vector<unsigned char> &input);
};
