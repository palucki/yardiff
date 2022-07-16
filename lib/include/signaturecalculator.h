#pragma once

#include "dataprovider.h"
#include "signature.h"

class SignatureCalculator
{
public:
    SignatureCalculator(DataProvider &data_provider);
    const Signature& calculate();
private:
    DataProvider& m_data_provider;
    Signature m_signature;
};

