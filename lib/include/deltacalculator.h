#pragma once

#include "dataprovider.h"
#include "signature.h"

class DeltaCalculator
{
public:
    DeltaCalculator(DataProvider &data_provider, const Signature& signature);
};
