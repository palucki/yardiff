#include <gtest/gtest.h>

#include "rollingchecksum.h"

TEST(RollingChecksum, TestBiggerNumbers)
{
    {
        RollingChecksum checksum;
        std::vector<unsigned char> data{100, 200, 123, 124, 213, 76, 127, 201, 12, 19, 99};
        EXPECT_EQ(checksum.calculate(data), 573965582);
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
        RollingChecksum checksum;
        std::vector<unsigned char> data{10, 20, 30};
        EXPECT_EQ(checksum.calculate(data), 6553660);
        EXPECT_EQ(checksum.roll(10, 40), 10485850);
        EXPECT_EQ(checksum.roll(20, 50), 14418040);
        EXPECT_EQ(checksum.roll(30, 60), 18350230);
    }
}

TEST(RollingChecksum, TestRollingOut)
{
    {
        RollingChecksum checksum;
        std::vector<unsigned char> data{0, 1, 2};
        EXPECT_EQ(checksum.calculate(data), 262147);
        EXPECT_EQ(checksum.roll(0, 0), 458755);
        EXPECT_EQ(checksum.roll(1, 1), 458755); //rolling 1 - 1 is a identity operation
        EXPECT_EQ(checksum.roll(2, 2), 262147); //back to initial value
    }
}

TEST(RollingChecksum, TestMultipleByteInputs)
{
    {
        RollingChecksum checksum;
        std::vector<unsigned char> data{0, 1};
        EXPECT_EQ(checksum.calculate(data), 65537);
    }

    {
        RollingChecksum checksum;
        std::vector<unsigned char> data{10, 10, 10, 10};
        EXPECT_EQ(checksum.calculate(data), 6553640);
    }
}

TEST(RollingChecksum, TestSingleByteInput)
{
    RollingChecksum checksum;

    {
        std::vector<unsigned char> data{0};
        EXPECT_EQ(checksum.calculate(data), 0);
    }

    {
        std::vector<unsigned char> data{100};
        EXPECT_EQ(checksum.calculate(data), 6553700);
    }

    {
        std::vector<unsigned char> data{0xFF};
        EXPECT_EQ(checksum.calculate(data), 16711935);
    }
}
