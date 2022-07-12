#include "strongchecksum.h"

#include <QCryptographicHash>
#include <QByteArray>
#include <QString>

std::string StrongChecksum::calculate(const std::vector<unsigned char>& input)
{
    const QByteArray input_array{reinterpret_cast<const char*>(input.data()), static_cast<int>(input.size())};
    const auto byte_data = QCryptographicHash::hash(input_array, QCryptographicHash::Md4);
    return std::string{byte_data.constData(), byte_data.constEnd()};
}
