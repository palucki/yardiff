#include <list>
#include <gtest/gtest.h>

#include "rollingchecksum.h"

namespace
{
const int MODULO_FACTOR = 1 << 16;
}

TEST(RollingChecksum, TestBiggerData)
{
    {
        std::vector<unsigned char> data{100, 200, 123, 124, 213, 76, 127, 201, 12, 19, 99};
        RollingChecksum checksum(data, 5, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 139592440);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 155058912);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 132973207);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 141230821);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 141820533);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 100532659);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 105644490);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 105644490);
    }
}

TEST(RollingChecksum, TestProperRolling)
{
    {
        std::vector<unsigned char> data{0, 1, 2};
        RollingChecksum checksum(data, 2, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 65537);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 262147);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 262147);
    }

    {
        std::vector<unsigned char> data{0, 1, 2, 3, 4};
        RollingChecksum checksum(data, 3, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 262147);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 655366);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 1048585);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 1048585);
    }
}

TEST(RollingChecksum, TestInputWithBlockSizeEqualToOneCanBeRolledToTheLimits)
{
    {
        std::vector<unsigned char> data{0, 100};
        RollingChecksum checksum(data, 1, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 0);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 6553700);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 6553700);
    }

    {
        std::vector<unsigned char> data{255, 10};
        RollingChecksum checksum(data, 1, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 16711935);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 655370);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 655370);
    }
}

TEST(RollingChecksum, TestMultipleByteInputsWithBlockSizeEqualToInputSize)
{
    {
        std::vector<unsigned char> data{0, 1};
        RollingChecksum checksum(data, 2, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 65537);
    }

    {
        std::vector<unsigned char> data{10, 10, 10, 10};
        RollingChecksum checksum(data, 4, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 6553640);
    }
}

TEST(RollingChecksum, TestSingleByteInput)
{
    {
        std::vector<unsigned char> data{0};
        RollingChecksum checksum(data, 1, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 0);
    }

    {
        std::vector<unsigned char> data{100};
        RollingChecksum checksum(data, 1, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 6553700);
    }

    {
        std::vector<unsigned char> data{0xFF};
        RollingChecksum checksum(data, 1, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 16711935);
    }
}

TEST(RollingChecksum, WhenDataSmallerThanBlockSizeReturnsZero)
{
    {
        std::vector<unsigned char> data{0};
        RollingChecksum checksum(data, 10, MODULO_FACTOR);

        EXPECT_EQ(checksum.currentValue(), 0);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 0);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 0);
    }

    {
        std::vector<unsigned char> data{1,2,3,4,5,6,7,8,9};
        RollingChecksum checksum(data, 10, MODULO_FACTOR);
        EXPECT_EQ(checksum.currentValue(), 0);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 0);
        checksum.roll();
        EXPECT_EQ(checksum.currentValue(), 0);
    }
}
