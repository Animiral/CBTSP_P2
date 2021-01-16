// tests for the Mouse Colony Optimization
#include "gtest/gtest.h"
#include <memory>

import mco;
import cbtsp;
import local;

class McoTest : public ::testing::Test
{

protected:

    // sensible parameters for all tests
    int ticks = 100; // number of iterations on a stagnated solution before termination
    int mice = 100; // number of traversals within a tick to construct solution candidates
    float evaporation = .1f; // fraction of pheromone decrease per tick
    float pheromoneAttraction = 10.f; // to which degree local pheromones attract
    float objectiveAttraction = 1.f; // to which degree local objective value attracts
    ReinforceStrategy reinforceStrategy = ReinforceStrategy::LAMARCK;
    std::shared_ptr<Random> random = std::make_shared<Random>(); // random number generator

    Mco buildMco(std::size_t vertices) const
    {
        auto improvement = buildImprovement(vertices);
        return Mco(ticks, mice, evaporation,
            pheromoneAttraction, objectiveAttraction,
            reinforceStrategy,
            random, move(improvement));
    }

    // construct the improvement heuristic
    static std::unique_ptr<LocalSearch> buildImprovement(std::size_t vertices)
    {
        auto neighborhood = std::make_unique<TwoExchangeNeighborhood>();
        auto step = std::make_unique<BestImprovement>(move(neighborhood));
        return std::make_unique<LocalSearch>(move(step));
    }

};

// Test that MCO can find a feasible solution, if available.
TEST_F(McoTest, BasicRun)
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

    auto mco = buildMco(problem.vertices());
    const Solution actual = mco.search(problem);
    EXPECT_TRUE(actual.isFeasible());
}

// Test that MCO can find a good solution, if available.
TEST_F(McoTest, EasyRun)
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
    auto mco = buildMco(problem.vertices());

    Solution actual = mco.search(problem);
    actual.normalize();
    EXPECT_EQ(actual.vertices(), optimum.vertices());
}

// Test that MCO is good at finding the best solution.
TEST_F(McoTest, QualityRun)
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
    auto mco = buildMco(problem.vertices());

    Solution actual = mco.search(problem);
    actual.normalize();
    EXPECT_EQ(actual.vertices(), optimum.vertices());
}
