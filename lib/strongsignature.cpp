#include "strongsignature.h"


#include <QCryptographicHash>
#include <QByteArray>
#include <QString>

std::string StrongSignature::signature(const std::string &input)
{
    const QByteArray input_array{input.c_str(), static_cast<int>(input.size())};
    const auto byte_data = QCryptographicHash::hash(input_array, QCryptographicHash::Md4);
    const QString str = byte_data.toHex();
    return str.toStdString();
}
