// tests for basic definitions and operations
#include "gtest/gtest.h"

import cbtsp;

// Ensure that problems are parsed correctly from a well-formed text.
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

class CbtspFixture : public ::testing::Test
{

protected:

    Problem problem;

    CbtspFixture() : problem(4ull)
    {
        problem.addEdge({ 0, 1, 1 });
        problem.addEdge({ 0, 2, -1 });
        problem.addEdge({ 1, 2, 3 });
        problem.addEdge({ 2, 3, 5 });
        problem.addEdge({ 3, 0, 0 });
    }

};

// Ensure that the objective value of the solution is
// correctly computed from the sum of edge values.
TEST_F(CbtspFixture, SolutionObjective)
{
    auto solution = Solution(problem, { 0, 1, 2 });
    EXPECT_EQ(3, solution.objective());
}

// Ensure that the string representation of the solution is as expected.
TEST_F(CbtspFixture, SolutionString)
{
    auto solution = Solution(problem, { 3, 0, 1 });
    EXPECT_EQ("3 0 1", solution.representation());
}

// Ensure that the the twoOpt move works.
TEST_F(CbtspFixture, TwoOpt)
{
    auto solution = Solution(problem, { 0, 1, 2, 3 });
    solution.twoOpt(2, 0);
    EXPECT_EQ("1 0 2 3", solution.representation());
}

// Ensure that the the solution is correctly normalized.
TEST_F(CbtspFixture, Normalize)
{
    auto solution = Solution(problem, { 3, 2, 1 });
    solution.normalize();
    EXPECT_EQ("1 2 3", solution.representation());
}

// Test the feasibility evaluation.
TEST_F(CbtspFixture, IsFeasible)
{
    EXPECT_TRUE(Solution(problem, { 0, 1, 2, 3 }).isFeasible());
    EXPECT_FALSE(Solution(problem, { 3, 0, 1 }).isFeasible());
    EXPECT_FALSE(Solution(problem, { 0, 2, 3, 1 }).isFeasible());
}
