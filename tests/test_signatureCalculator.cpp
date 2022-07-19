#include <gtest/gtest.h>

#include "signaturecalculator.h"
#include "strongchecksum.h"
#include "mockdataprovider.h"

TEST(SignatureCalculator, TestDisallowedBlockSizeDoesNotCalculate)
{
    MockDataProvider data_provider{{}, 0};
    SignatureCalculator calculator{data_provider};
    const auto signature = calculator.calculate();
    EXPECT_EQ(signature.empty(), true);
}

TEST(SignatureCalculator, TestEmptyInputIsDiscarded)
{
    MockDataProvider data_provider{{}, 1};
    SignatureCalculator calculator{data_provider};
    const auto signature = calculator.calculate();

    EXPECT_EQ(signature.size(), 0);
}

TEST(SignatureCalculator, TestSingleByteInputDifferentAllowedBlockSizes)
{
    {
        MockDataProvider data_provider{{0}, 1};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();

        Signature expected_signature {
            {0, {BlockData{StrongChecksum::calculate({0}), 0}}}
        };

        EXPECT_EQ(signature, expected_signature);
    }

    {
        Signature expected_signature {
            {6553700, {BlockData{StrongChecksum::calculate({100}), 0}}}
        };

        {
            MockDataProvider data_provider{{100}, 1};
            SignatureCalculator calculator{data_provider};
            const auto signature = calculator.calculate();
            EXPECT_EQ(signature, expected_signature);
        }

        {
            MockDataProvider data_provider{{100}, 2};
            SignatureCalculator calculator{data_provider};
            const auto signature = calculator.calculate();
            EXPECT_EQ(signature, expected_signature);
        }

        {
            MockDataProvider data_provider{{100}, 600};
            SignatureCalculator calculator{data_provider};
            const auto signature = calculator.calculate();
            EXPECT_EQ(signature, expected_signature);
        }
    }
}

TEST(SignatureCalculator, TestRepeatedBlocksDoNotCreateNewEntries)
{
    {
        MockDataProvider data_provider{{'a','b','b','a','c','a'}, 1};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();
        Signature expected_signature {
            {6357089, {BlockData{StrongChecksum::calculate({'a'}), 0}}},
            {6422626, {BlockData{StrongChecksum::calculate({'b'}), 1}}},
            {6488163, {BlockData{StrongChecksum::calculate({'c'}), 4}}},
        };
        EXPECT_EQ(signature, expected_signature);
    }

    {
        MockDataProvider data_provider{{'a','l','a','m','a','t','a','l','a','m','a','t','a'}, 3};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();

        Signature expected_signature {
            {39584046, {BlockData{StrongChecksum::calculate({'a','l','a'}), 0}}},
            {41746754, {BlockData{StrongChecksum::calculate({'m','a','t'}), 1}}},
            {6357089, {BlockData{StrongChecksum::calculate({'a'}), 4}}},
        };
        EXPECT_EQ(signature, expected_signature);
    }


}

TEST(SignatureCalculator, TestTheSameBlockBetweenBlocksBoundaryWillBeTreatedAsDifferentBlock)
{
    {
        //here first "ab" is within block boundary, and the second one is between
        MockDataProvider data_provider{{'a','b','c','a','b', 'd'}, 2};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();

        Signature expected_signature {
            {19136707, {BlockData{StrongChecksum::calculate({'a','b'}), 0}}},
            {19333316, {BlockData{StrongChecksum::calculate({'c','a'}), 1}}},
            {19398854, {BlockData{StrongChecksum::calculate({'b','d'}), 2}}},
        };
        EXPECT_EQ(signature, expected_signature);
    }
}

TEST(SignatureCalculator, TestBlockSizeGreaterThanInputSize)
{
    {
        MockDataProvider data_provider{{'a','b','c','d','e','f'}, 10};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();

        Signature expected_signature {
            {135791189, {BlockData{StrongChecksum::calculate({'a','b','c','d','e','f'}), 0}}},
        };
        EXPECT_EQ(signature, expected_signature);
    }
}

TEST(SignatureCalculator, TestWhenBlockSizeSmallerThanInputSizeAllBytesAreProcessed)
{
    {
        MockDataProvider data_provider{{'a','b','c','d','e','f'}, 5};
        SignatureCalculator calculator{data_provider};
        const auto signature = calculator.calculate();

        Signature expected_signature {
            {96666095, {BlockData{StrongChecksum::calculate({'a','b','c','d','e'}), 0}}},
            {6684774, {BlockData{StrongChecksum::calculate({'f'}), 1}}},
        };
        EXPECT_EQ(signature, expected_signature);
    }
}
