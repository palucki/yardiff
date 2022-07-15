#pragma once

#include "dataprovider.h"
#include "signature.h"

class SignatureCalculator
{
public:
    SignatureCalculator(DataProvider &data_provider, const int modulo_factor);
    const Signature& calculate();
private:
    DataProvider& m_data_provider;
    const int m_modulo_factor;
    Signature m_signature;
};

