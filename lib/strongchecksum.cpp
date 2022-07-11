#include "strongchecksum.h"

#include <QCryptographicHash>
#include <QByteArray>
#include <QString>

std::vector<unsigned char> StrongChecksum::calculate(const std::vector<unsigned char>& input)
{
    const QByteArray input_array{reinterpret_cast<const char*>(input.data()), static_cast<int>(input.size())};
    const auto byte_data = QCryptographicHash::hash(input_array, QCryptographicHash::Md4);
    return std::vector<unsigned char>(byte_data.constData(), byte_data.constEnd());
}
