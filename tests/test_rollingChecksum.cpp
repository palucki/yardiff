#include <list>
#include <gtest/gtest.h>

#include "rollingchecksum.h"

namespace
{
const int MODULO_FACTOR = 1 << 16;
}

TEST(RollingChecksum, TestBiggerNumbers)
{
    {
        std::vector<unsigned char> data{100, 200, 123, 124, 213, 76, 127, 201, 12, 19, 99};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 573965582);
        EXPECT_EQ(checksum.roll(data[0], 213), 594085247);
        EXPECT_EQ(checksum.roll(data[1], 177), 540607848);
        EXPECT_EQ(checksum.roll(data[2], 203), 547882424);
        EXPECT_EQ(checksum.roll(data[3], 1),   546374973);
        EXPECT_EQ(checksum.roll(data[4], 12),  467534964);
        EXPECT_EQ(checksum.roll(data[5], 17),  483591225);
        EXPECT_EQ(checksum.roll(data[6], 69),  459080703);
    }
}

TEST(RollingChecksum, TestRolling)
{
    {
        std::vector<unsigned char> data{10, 20, 30};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 6553660);
        checksum.roll(10, 40);
        EXPECT_EQ(checksum.value(), 10485850);
        checksum.roll(20, 50);
        EXPECT_EQ(checksum.value(), 14418040);
        checksum.roll(30, 60);
        EXPECT_EQ(checksum.value(), 18350230);
    }
}

TEST(RollingChecksum, TestRollingOut)
{
    {
        std::vector<unsigned char> data{0, 1, 2};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 262147);
        checksum.roll(0, 0);
        EXPECT_EQ(checksum.value(), 458755);
        checksum.roll(1, 1);
        EXPECT_EQ(checksum.value(), 458755); //rolling 1 - 1 is a identity operation
        checksum.roll(2, 2);
        EXPECT_EQ(checksum.value(), 262147); //back to initial value
    }
}

TEST(RollingChecksum, TestMultipleByteInputs)
{
    {
        std::vector<unsigned char> data{0, 1};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 65537);
    }

    {
        std::vector<unsigned char> data{10, 10, 10, 10};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 6553640);
    }
}

TEST(RollingChecksum, TestSingleByteInput)
{
    {
        std::vector<unsigned char> data{0};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 0);
    }

    {
        std::vector<unsigned char> data{100};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 6553700);
    }

    {
        std::vector<unsigned char> data{0xFF};
        RollingChecksum checksum(data, MODULO_FACTOR);
        EXPECT_EQ(checksum.value(), 16711935);
    }
}
