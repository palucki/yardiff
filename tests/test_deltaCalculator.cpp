#include <gtest/gtest.h>

#include "deltacalculator.h"
#include "rollingchecksum.h"
#include "strongchecksum.h"
#include "mockdataprovider.h"
#include "signature.h"

TEST(DeltaCalculator, EmptyBaseData)
{
    {
        Signature signature;
        MockDataProvider data_provider({}, 5);

        DeltaCalculator calculator{data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "");
    }

    {
        Signature signature;
        MockDataProvider data_provider({'t','e','s','t'}, 2);

        DeltaCalculator calculator{data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "test");
    }

    {
        Signature signature;
        MockDataProvider data_provider({'t','e','s','t'}, 5);

        DeltaCalculator calculator{data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "test");
    }
}

TEST(DeltaCalculator, UpdatedDataEqualToBaseData)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'a','b','c'}), {StrongChecksum::calculate({'a','b','c'}), 0}},
        };
        MockDataProvider updated_data_provider({'a','b','c'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "{BLOCK_0}");
    }

    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'a','b','c'}), {StrongChecksum::calculate({'a','b','c'}), 0}},
            {r.calculate({'c','d','e'}), {StrongChecksum::calculate({'c','d','e'}), 1}},
        };

        MockDataProvider updated_data_provider({'a','b','c', 'c','d','e'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "{BLOCK_0}{BLOCK_1}");
    }
}

TEST(DeltaCalculator, UpdatedDataCompletelyDifferentThanNonEmptyBaseData)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'d','e','f'}), {StrongChecksum::calculate({'d','e','f'}), 0}},
        };
        MockDataProvider updated_data_provider({'a','b','c'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "abc");
    }

    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'d','e','f'}), {StrongChecksum::calculate({'d','e','f'}), 0}},
        };
        MockDataProvider updated_data_provider({'a','b','c','d','e','f','g','h','i'}, 5);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "abcdefghi");
    }
}

TEST(DeltaCalculator, UpdatedDataContainsOnlyReusedBlocks)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'d','e','f'}), {StrongChecksum::calculate({'d','e','f'}), 0}},
        };
        MockDataProvider updated_data_provider({'d','e','f','d','e','f','d','e','f'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "{BLOCK_0}{BLOCK_0}{BLOCK_0}");
    }
}

TEST(DeltaCalculator, UpdatedDataAddsData)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'d','e','f'}), {StrongChecksum::calculate({'d','e','f'}), 0}},
        };
        MockDataProvider updated_data_provider({'a','d','e','f','i'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "a{BLOCK_0}i");
    }

    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v'}), {StrongChecksum::calculate({'r','t','v'}), 0}},
            {r.calculate({'a','g','d'}), {StrongChecksum::calculate({'a','g','d'}), 1}},
        };
        MockDataProvider updated_data_provider({'r','t','v','e','u','r','o','a','g','d'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "{BLOCK_0}euro{BLOCK_1}");
    }
}

TEST(DeltaCalculator, UpdatedDataRemovesData)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v'}), {StrongChecksum::calculate({'r','t','v'}), 0}},
            {r.calculate({'a','g','d'}), {StrongChecksum::calculate({'a','g','d'}), 1}},
        };
        MockDataProvider updated_data_provider({'t','v','a','g','d'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "tv{BLOCK_1}");
    }

    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v'}), {StrongChecksum::calculate({'r','t','v'}), 0}},
            {r.calculate({'a','g','d'}), {StrongChecksum::calculate({'a','g','d'}), 1}},
        };
        MockDataProvider updated_data_provider({'r','t','v','a','g'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "{BLOCK_0}ag");
    }
}


TEST(DeltaCalculator, UpdatedDataChangesData)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v'}), {StrongChecksum::calculate({'r','t','v'}), 0}},
            {r.calculate({'a','g','d'}), {StrongChecksum::calculate({'a','g','d'}), 1}},
        };
        MockDataProvider updated_data_provider({'a','l','v','a','r','d'}, 3);

        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "alvard");
    }
}

TEST(DeltaCalculator, AllTypesOfOperationsSimultanously)
{
    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v','a','g','d'}), {StrongChecksum::calculate({'r','t','v','a','g','d'}), 0}},
            {r.calculate({'m','a','m','m','o','c'}), {StrongChecksum::calculate({'m','a','m','m','o','c'}), 1}},
            {r.calculate({'p','j','x','r','l','z'}), {StrongChecksum::calculate({'p','j','x','r','l','z'}), 2}},
            {r.calculate({'a','l','a','k','o','t'}), {StrongChecksum::calculate({'a','l','a','k','o','t'}), 3}},
        };

        std::string input = "!alrtvagdmammozmammoc1pjxrlzrtvagdalamamalaalakotalkot";
        std::vector<unsigned char> updated_data(input.begin(), input.end());
        MockDataProvider updated_data_provider(updated_data, 6);
        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "!al{BLOCK_0}mammoz{BLOCK_1}1{BLOCK_2}{BLOCK_0}alamamala{BLOCK_3}alkot");
    }

    {
        RollingChecksum r;
        Signature signature{
            {r.calculate({'r','t','v','a','g','d'}), {StrongChecksum::calculate({'r','t','v','a','g','d'}), 0}},
            {r.calculate({'m','a','m','m','o','c'}), {StrongChecksum::calculate({'m','a','m','m','o','c'}), 1}},
            {r.calculate({'p','j','x','r','l','z'}), {StrongChecksum::calculate({'p','j','x','r','l','z'}), 2}},
            {r.calculate({'a','l','a','k','o','t'}), {StrongChecksum::calculate({'a','l','a','k','o','t'}), 3}},
        };

        std::string input = "asdsdlfkpjxrlzsdlfksaalakotasdlaksd";
        std::vector<unsigned char> updated_data(input.begin(), input.end());
        MockDataProvider updated_data_provider(updated_data, 6);
        DeltaCalculator calculator{updated_data_provider, signature};
        const Delta delta = calculator.calculate();
        EXPECT_EQ(delta, "asdsdlfk{BLOCK_2}sdlfksa{BLOCK_3}asdlaksd");
    }
}
