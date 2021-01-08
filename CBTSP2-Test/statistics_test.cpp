// tests for statistics collection
#include "gtest/gtest.h"
#include <chrono>

import statistics;
import cbtsp;

// Ensure that statistics are correctly computed from records.
TEST(Statistics, BasicRun)
{
    auto problem = Problem{ 5ull, 100l };
    problem.addEdge({ 0, 1, 0 });
    problem.addEdge({ 1, 2, 0 });
    problem.addEdge({ 2, 3, 0 });
    problem.addEdge({ 3, 4, 0 });
    problem.addEdge({ 4, 0, 0 });
    problem.addEdge({ 0, 2, 1 });
    problem.addEdge({ 1, 3, 1 });

    auto statistics = Statistics("test");

    using namespace std::literals::chrono_literals;

    statistics.record(Solution{ problem, {0, 1, 2, 3, 4 } }, 1s); // value 0
    statistics.record(Solution{ problem, {0, 2, 1, 3, 4 } }, 3s); // value 2
    statistics.record(Solution{ problem, {0, 1, 2, 4, 3 } }, 1s); // 2 infeasible edges
    statistics.record(Solution{ problem, {0, 3, 1, 4, 2 } }, 3s); // 3 infeasible edges

    EXPECT_EQ("test", statistics.name());
    EXPECT_EQ(4, statistics.samples());
    EXPECT_EQ(2, statistics.feasibles());
    EXPECT_EQ("0 1 2 3 4", statistics.bestSolution()->representation());
    EXPECT_NEAR(1.f, statistics.meanObjective(), .001f);
    EXPECT_NEAR(2.f, statistics.stdevObjective(), .001f); // sqrt( 1/(2-1.5) * (1+1) )
    EXPECT_NEAR(2.5f, statistics.meanInfEdges(), .001f);
    EXPECT_NEAR(1.f, statistics.stdevInfEdges(), .001f); // sqrt( 1/(2-1.5) * (0.25+0.25) )
    EXPECT_EQ(2s, statistics.medRuntime(), .001f);
}
