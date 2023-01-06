#include "pch.h"

bool IsLeapYear(unsigned int year) {

    if (year % 400 == 0)
        return true;

    if (year % 100 == 0)
        return false;

    if (year % 4 == 0)
        return true;

    return false;
}

TEST(IsLeapYearTest, IsDivisibleBy400_returnsTrue) {
    EXPECT_EQ(IsLeapYear(1600), true);
}

TEST(IsLeapYearTest, IsDivisibleBy100_andNotDivisibleBy400_returnsFalse) {
    EXPECT_EQ(IsLeapYear(1700), false);
}

TEST(IsLeapYearTest, IsNotDivisibleBy4_returnsFalse) {
    EXPECT_EQ(IsLeapYear(103), false);
}

TEST(IsLeapYearTest, IsDivisibleBy4_andNotDivisibleBy100_returnsTrue) {
    EXPECT_EQ(IsLeapYear(1704), true);
}

