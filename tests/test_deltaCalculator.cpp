#include <gtest/gtest.h>

#include "deltacalculator.h"
#include "strongchecksum.h"
#include "mockdataprovider.h"
#include "signature.h"

TEST(DeltaCalculator, Basic)
{
    Signature signature;
    MockDataProvider data_provider({}, 0);

    DeltaCalculator calculator{data_provider, signature};
}
