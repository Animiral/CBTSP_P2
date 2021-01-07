// tests for basic definitions and operations
#include "gtest/gtest.h"

import cbtsp;

class CbtspTest : public ::testing::Test
{

protected:

    Problem problem;

    CbtspTest() : problem(4ull, 100l)
    {
        problem.addEdge({ 0, 1, 1 });
        problem.addEdge({ 0, 2, -1 });
        problem.addEdge({ 1, 2, 3 });
        problem.addEdge({ 2, 3, 5 });
        problem.addEdge({ 3, 0, 0 });
    }

};

// Ensure that problems are parsed correctly from a well-formed text.
TEST_F(CbtspTest, FromText)
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

// Ensure that the problem's big-M is properly calculated.
TEST_F(CbtspTest, BigM)
{
    const auto text = "4 5\n0 1 1\n0 2 -1\n1 2 3\n2 3 5\n3 0 0\n";
    const auto problem = Problem::fromText(text);
    EXPECT_EQ(10, problem.bigM());
    EXPECT_EQ(10, problem.value(3, 1));
}

// Ensure that the objective value of the solution is
// correctly computed from the sum of edge values.
TEST_F(CbtspTest, SolutionObjective)
{
    auto solution = Solution(problem, { 0, 1, 2 });
    EXPECT_EQ(3, solution.objective());
}

// Ensure that the string representation of the solution is as expected.
TEST_F(CbtspTest, SolutionString)
{
    auto solution = Solution(problem, { 3, 0, 1 });
    EXPECT_EQ("3 0 1", solution.representation());
}

// Ensure that the the twoOpt move works.
TEST_F(CbtspTest, TwoOpt)
{
    auto solution = Solution(problem, { 0, 1, 2, 3 });
    solution.twoOpt(2, 0);
    EXPECT_EQ("1 0 2 3", solution.representation());
    EXPECT_EQ(105, solution.value());
}

// Ensure that the the solution is correctly normalized.
TEST_F(CbtspTest, Normalize)
{
    auto solution = Solution(problem, { 3, 2, 1 });
    solution.normalize();
    EXPECT_EQ("1 2 3", solution.representation());
}

// Test the feasibility evaluation.
TEST_F(CbtspTest, IsFeasible)
{
    EXPECT_TRUE(Solution(problem, { 0, 1, 2, 3 }).isFeasible());
    EXPECT_FALSE(Solution(problem, { 3, 0, 1 }).isFeasible());
    EXPECT_FALSE(Solution(problem, { 0, 2, 3, 1 }).isFeasible());
}
