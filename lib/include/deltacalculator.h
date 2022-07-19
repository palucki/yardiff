#pragma once

#include "dataprovider.h"
#include "signature.h"
#include "delta.h"

/*!
 * \brief The DeltaCalculator class is responsible for computing the detla, which represents a workflow
 * to convert base data into updated data.
 */

class DeltaCalculator
{
public:
    DeltaCalculator(DataProvider &data_provider, const Signature& signature);

    //! computes delta based on the given base signature and updated data from provider.
    Delta calculate();
private:
    DataProvider& m_data_provider;
    const Signature m_signature;
};
