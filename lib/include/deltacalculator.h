#pragma once

#include "dataprovider.h"
#include "signature.h"
#include "delta.h"

class DeltaCalculator
{
public:
    DeltaCalculator(DataProvider &data_provider, const Signature& signature);

    //! computes delta based on the given signature and updated data from provider.
    Delta calculate();
private:
    DataProvider& m_data_provider;
    const Signature m_signature;
};
