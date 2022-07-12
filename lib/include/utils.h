#pragma once

#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace utils
{

std::string bytesToHex(const std::vector<unsigned char>& data)
{
    std::stringstream ss;
    for(unsigned int d : data)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << d;
    }
    return ss.str();
}

}
