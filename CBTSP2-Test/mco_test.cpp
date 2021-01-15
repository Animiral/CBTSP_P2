// tests for the Mouse Colony Optimization
#include "gtest/gtest.h"

import mco;
import cbtsp;

// Test that MCO can find a feasible solution, if available.
TEST(Mco, BasicRun)
{
    // We provide a small problem with few edges.
    // It should not be hard to find a solution using the available edges.
    auto problem = Problem{ 5, 10000l };
    problem.addEdge({ 0, 1, 1000 });
    problem.addEdge({ 1, 2, -1000 });
    problem.addEdge({ 2, 3, 500 });
    problem.addEdge({ 3, 4, 200 });
    problem.addEdge({ 4, 0, -200 });
    problem.addEdge({ 0, 2, -500 });

    int ticks = 1000; // number of iterations on a stagnated solution before termination
    int mice = 100; // number of traversals within a tick to construct solution candidates
    float evaporation = .1f; // fraction of pheromone decrease per tick
    float pheromoneAttraction = 10.f; // to which degree local pheromones attract
    float objectiveAttraction = 1.f; // to which degree local objective value attracts
    auto random = std::make_shared<Random>(); // random number generator

    auto mco = Mco(ticks, mice, evaporation, pheromoneAttraction, objectiveAttraction, random);
    const Solution actual = mco.search(problem);
    EXPECT_TRUE(actual.isFeasible());
}

// Test that MCO can find a good solution, if available.
TEST(Mco, EasyRun)
{
    // We provide a problem instance with lots of edges,
    // but only one truly excellent solution.
    // The expectation is that the Mouse Colony Optimization heuristic will
    // find the optimal solution by discarding the ridiculously bad choices.

    auto problem = Problem{ 6, 10000l };
    // best edges
    problem.addEdge({ 0, 1, 1000 });
    problem.addEdge({ 1, 2, -1000 });
    problem.addEdge({ 2, 3, 500 });
    problem.addEdge({ 3, 4, 200 });
    problem.addEdge({ 4, 5, -200 });
    problem.addEdge({ 5, 0, -500 });

    // bad edges
    problem.addEdge({ 0, 2, 1501 });
    problem.addEdge({ 1, 3, 701 });
    problem.addEdge({ 2, 4, 701 });
    problem.addEdge({ 3, 5, 1499 });
    problem.addEdge({ 4, 0, 1199 });
    problem.addEdge({ 5, 1, 799 });
    problem.addEdge({ 0, 3, 702 });
    problem.addEdge({ 1, 4, 1502 });
    problem.addEdge({ 2, 5, 1202 });

    const auto optimum = Solution{ problem, {0, 1, 2, 3, 4, 5} };

    int ticks = 1000; // number of iterations on a stagnated solution before termination
    int mice = 100; // number of traversals within a tick to construct solution candidates
    float evaporation = .1f; // fraction of pheromone decrease per tick
    float pheromoneAttraction = 10.f; // to which degree local pheromones attract
    float objectiveAttraction = 1.f; // to which degree local objective value attracts
    auto random = std::make_shared<Random>(); // random number generator

    auto mco = Mco(ticks, mice, evaporation, pheromoneAttraction, objectiveAttraction, random);

    Solution actual = mco.search(problem);
    actual.normalize();
    EXPECT_EQ(actual.vertices(), optimum.vertices());
}

// Test that MCO is good at finding the best solution.
TEST(Mco, QualityRun)
{
    // We provide a problem instance with lots of edges and lots of solutions
    // with rather similar values (<5% difference).
    // The expectation is that the Mouse Colony Optimization heuristic will
    // find the optimal solution, given a not-too-excessive number of
    // iterations and mice to run.

    auto problem = Problem{ 6, 10000l };
    // best edges
    problem.addEdge({ 0, 1, 1000 });
    problem.addEdge({ 1, 2, -1000 });
    problem.addEdge({ 2, 3, 500 });
    problem.addEdge({ 3, 4, 200 });
    problem.addEdge({ 4, 5, -200 });
    problem.addEdge({ 5, 0, -500 });

    // bad edges
    problem.addEdge({ 0, 2, 501 });
    problem.addEdge({ 1, 3, 201 });
    problem.addEdge({ 2, 4, 1001 });
    problem.addEdge({ 3, 5, 499 });
    problem.addEdge({ 4, 0, 199 });
    problem.addEdge({ 5, 1, 999 });
    problem.addEdge({ 0, 3, 1002 });
    problem.addEdge({ 1, 4, 502 });
    problem.addEdge({ 2, 5, 202 });

    const auto optimum = Solution{ problem, {0, 1, 2, 3, 4, 5} };

    int ticks = 100; // number of iterations on a stagnated solution before termination
    int mice = 100; // number of traversals within a tick to construct solution candidates
    float evaporation = .5f; // fraction of pheromone decrease per tick
    float pheromoneAttraction = 1000.f; // to which degree local pheromones attract
    float objectiveAttraction = 1.f; // to which degree local objective value attracts
    auto random = std::make_shared<Random>(); // random number generator

    auto mco = Mco(ticks, mice, evaporation, pheromoneAttraction, objectiveAttraction, random);

    Solution actual = mco.search(problem);
    actual.normalize();
    EXPECT_EQ(actual.vertices(), optimum.vertices());
}
