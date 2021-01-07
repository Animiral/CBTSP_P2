// tests for Local Search functions
#include "gtest/gtest.h"
#include <vector>
#include <span>
#include <ranges>
#include <iterator>
#include <utility>
#include <memory>

import cbtsp;
import local;

class LocalTest : public ::testing::Test
{

protected:

    Problem problem;

    LocalTest() : problem(5ull, 100l)
    {
        problem.addEdge({ 0, 1, 1 });
        problem.addEdge({ 1, 2, -1 });
        problem.addEdge({ 2, 3, 3 });
        problem.addEdge({ 3, 4, -1 });
        problem.addEdge({ 4, 0, -2 });
        problem.addEdge({ 0, 3, 3 });
        problem.addEdge({ 1, 4, -4 });
    }

};

// Ensure that the two-exchange neighborhood is covered as expected
TEST_F(LocalTest, TwoExchangeNeighborhood)
{
    auto solution = Solution(problem, { 0, 1, 2, 3, 4 });

    // list of neighbors that we expect to find in this neighborhood
    std::vector<std::vector<Vertex>> expected = {
        { 0, 1, 2, 4, 3 },
        { 0, 1, 3, 2, 4 },
        { 0, 1, 4, 3, 2 },
        { 0, 2, 1, 3, 4 },
        { 0, 3, 2, 1, 4 }
    };

    for (auto it = TwoExchangeNeighborhood(5); it != std::default_sentinel; ++it) {
        Solution s = it.applyCopy(solution);
        s.normalize();
        auto found = std::ranges::find(expected, s.vertices());
        ASSERT_NE(expected.end(), found);
        expected.erase(found);
    }

    EXPECT_TRUE(expected.empty());
}

// Ensure that the vertex shift neighborhood (very narrow two-exchange neighborhood)
// is covered as expected
TEST_F(LocalTest, VertexShiftNeighborhood)
{
    auto solution = Solution(problem, { 0, 1, 2, 3, 4 });

    // list of neighbors that we expect to find in this neighborhood
    std::vector<std::vector<Vertex>> expected = {
        { 0, 1, 2, 4, 3 },
        { 0, 1, 3, 2, 4 },
        { 0, 1, 4, 3, 2 },
        { 0, 2, 1, 3, 4 },
        { 0, 3, 2, 1, 4 }
    };

    for (auto it = TwoExchangeNeighborhood(5, 2, 2); it != std::default_sentinel; ++it) {
        Solution s = it.applyCopy(solution);
        s.normalize();
        auto found = std::ranges::find(expected, s.vertices());
        ASSERT_NE(expected.end(), found);
        expected.erase(found);
    }

    EXPECT_TRUE(expected.empty());
}

TEST_F(LocalTest, Search)
{
    const auto optimum = Solution(problem, { 0, 1, 2, 3, 4 });
    auto start = Solution(problem, { 0, 1, 3, 4, 2 }); // 2 steps from optimum

    auto neighborhood = std::make_unique<TwoExchangeNeighborhood>(5);
    auto step = std::make_unique<BestImprovement>(move(neighborhood));
    auto search = LocalSearch(move(step), WhenStagnant{});
    auto actual = search.search(std::move(start));
    actual.normalize();

    EXPECT_EQ(optimum.vertices(), actual.vertices());
}
