#include "strongchecksum.h"

#include <sstream>
#include <iomanip>

#include <QCryptographicHash>
#include <QByteArray>
#include <QString>

std::string StrongChecksum::calculate(const std::vector<unsigned char>& input)
{
    const QByteArray input_array{reinterpret_cast<const char*>(input.data()), static_cast<int>(input.size())};
    const auto byte_data = QCryptographicHash::hash(input_array, QCryptographicHash::Md4);

    std::stringstream ss;
    for(const auto d : byte_data)
    {
        //the little '+' fellow does the trick to print char as value, instead of character
        ss << std::hex << std::setw(2) << std::setfill('0') << +(unsigned char)d;
    }
    return ss.str();
}
