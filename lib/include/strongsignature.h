#pragma once

#include <string>

/*!
 * \brief The StrongSignature class implements MD4 hash function
 */

class StrongSignature
{
public:
    static std::string signature(const std::string& input);
};
