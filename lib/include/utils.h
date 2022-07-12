#pragma once

#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace utils
{

template<typename T>
std::string bytesToHex(const T& data)
{
    std::stringstream ss;
    for(const auto d : data)
    {
        //the little '+' fellow does the trick to print char as value, instead of character
        ss << std::hex << std::setw(2) << std::setfill('0') << +(unsigned char)d;
    }
    return ss.str();
}

}
