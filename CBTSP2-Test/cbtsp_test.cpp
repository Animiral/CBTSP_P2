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

// Ensure that the problem's big-M is properly calculated.
TEST(Cbtsp, BigM)
{
    auto problem = Problem(4ull);
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

//def setUp(self) :
//    self.instance = Instance(4)
//    self.instance.add_edge(Edge(0, 1, 1))
//    self.instance.add_edge(Edge(0, 2, -1))
//    self.instance.add_edge(Edge(1, 2, 3))
//    self.instance.add_edge(Edge(2, 3, 5))
//    self.instance.add_edge(Edge(3, 0, 0))

// Ensure that the value of the solution is correctly computed from the sum of edge values.
TEST(Cbtsp, SolutionValue)
{
    //solution = Solution(self.instance, [0, 1, 2])
    //self.assertEqual(3, solution.value)
}

// Ensure that the the solution is correctly normalized.
TEST(Cbtsp, Normalize)
{
    //solution = Solution(self.instance, [3, 2, 1])
    //self.assertEqual([1, 2, 3], solution.normalized().vertices)
}

// Ensure that the string representation of the solution is as expected.
TEST(Cbtsp, SolutionString)
{
    //solution = Solution(self.instance, [3, 0, 1])
    //self.assertEqual("3 0 1", str(solution))
}

// Test the feasibility evaluation.
TEST(Cbtsp, IsFeasible)
{
    //self.assertTrue(Solution(self.instance, [0, 1, 2, 3]).is_feasible())
    //self.assertFalse(Solution(self.instance, [3, 0, 1]).is_feasible())
    //self.assertFalse(Solution(self.instance, [0, 2, 3, 1]).is_feasible())
}
