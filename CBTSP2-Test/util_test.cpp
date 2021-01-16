// tests for utility functions
#include "gtest/gtest.h"
#include <vector>
#include <string>

import util;

// Ensure that string formatting works
TEST(UtilTest, Format)
{
	EXPECT_EQ("a 1 b 2 c 3", format("a {} b {} c {}", 1, 2, 3));
}

// Ensure that string joining works
TEST(UtilTest, Join)
{
	const std::vector<std::string> parts = { "hello", "joining", "world" };
	EXPECT_EQ("hello - joining - world", join(" - ", parts));
}
