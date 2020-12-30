// tests for basic definitions and operations
#include "gtest/gtest.h"

import cbtsp;

// Ensure that instances are parsed correctly from a well-formed text.
TEST(Cbtsp, FromText)
{
    const auto text = "3 3\n0 1 1\n0 2 -1\n1 2 3\n";
    const auto problem = Problem::fromText(text);
    EXPECT_EQ(3, problem.vertices());
    EXPECT_EQ(1, problem.value(0, 1));
    EXPECT_EQ(1, problem.value(1, 0));
    EXPECT_EQ(-1, problem.value(0, 2));
    EXPECT_EQ(-1, problem.value(2, 0));
    EXPECT_EQ(3, problem.value(1, 2));
    EXPECT_EQ(3, problem.value(2, 1));
}

// Ensure that the instance's big_m is properly calculated.
TEST(Cbtsp, BigM)
{
    auto problem = Problem(4);
    problem.addEdge({ 0, 1, 1 });
    problem.addEdge({ 1, 2, 3 });
    problem.addEdge({ 2, 3, -1 });
    problem.addEdge({ 3, 0, -4 });
    problem.addEdge({ 0, 2, 0 });
    EXPECT_EQ(10, problem.bigM());
    EXPECT_EQ(10, problem.value(1, 3));
    problem.calculateBigM();
    EXPECT_EQ(8, problem.bigM());
    EXPECT_EQ(8, problem.value(3, 1));
}
