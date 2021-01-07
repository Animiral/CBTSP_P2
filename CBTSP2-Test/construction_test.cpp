// tests for construction heuristics
#include "gtest/gtest.h"
#include <algorithm>
#include <random>

import construction;

class ConstructionTest : public ::testing::Test
{

protected:

    Problem problem;

    ConstructionTest() : problem(5ull, 100l)
    {
        problem.addEdge({ 0, 1, 1 });
        problem.addEdge({ 1, 2, 2 });
        problem.addEdge({ 2, 3, 3 });
        problem.addEdge({ 3, 4, -1 });
        problem.addEdge({ 4, 0, -2 });
    }

};

// Ensure that vertex selection happens according to the RNG.
TEST_F(ConstructionTest, SelectRandom)
{
    struct
    {
        using result_type = unsigned int;
        static result_type min() { return 0; }
        static result_type max() { return 1000; }
        result_type operator()() { return 4; } // chosen by fair dice roll. guaranteed to be random.
    } mockRng;
    auto selector = RandomSelector(mockRng);
    auto partialSolution = Solution(problem, { 0, 2 });
    Vertex availables[] = { 1, 3, 4 };

    auto selection = selector.select(problem, partialSolution);
    EXPECT_NE(availables + 3, std::find(availables, availables + 3, selection));
    partialSolution.insert(2, selection);
    EXPECT_EQ(availables + 2, std::remove(availables, availables + 3, selection));

    selection = selector.select(problem, partialSolution);
    EXPECT_NE(availables + 2, std::find(availables, availables + 2, selection));
    partialSolution.insert(3, selection);
    EXPECT_EQ(availables + 1, std::remove(availables, availables + 2, selection));

    selection = selector.select(problem, partialSolution);
    EXPECT_EQ(availables[0] , selection);
}

// Ensure that the farthest vertex is always selected.
TEST_F(ConstructionTest, SelectFarthestCity)
{
    auto selector = FarthestCitySelector();
    EXPECT_EQ(0, selector.select(problem, Solution(problem, {})));
    EXPECT_EQ(3, selector.select(problem, Solution(problem, { 1, 2 })));
    EXPECT_EQ(3, selector.select(problem, Solution(problem, { 0, 2 })));
    EXPECT_EQ(2, selector.select(problem, Solution(problem, { 0, 3, 4 })));
}

// Ensure that inserting places the new vertex at the best spot in the tour.
TEST_F(ConstructionTest, BestTourInserter)
{
    auto inserter = BestTourInserter();
    auto partialSolution = Solution(problem, { 0, 2, 4 });
    inserter.insert(problem, partialSolution, 3);
    EXPECT_EQ("0 2 3 4", partialSolution.representation());
}

// Ensure that random construction results in a full-length solution.
TEST_F(ConstructionTest, ConstructRandom)
{
    auto selector = RandomSelector(std::default_random_engine{});
    auto inserter = BestTourInserter();
    auto construction = RandomConstruction(selector, inserter);
    auto solution = construction.construct(problem);
    EXPECT_FALSE(solution.isPartial());
}

// Ensure that deterministic construction results in a full-length solution.
TEST_F(ConstructionTest, ConstructDeterministic)
{
    auto selector = FarthestCitySelector();
    auto inserter = BestTourInserter();
    auto construction = DeterministicConstruction(selector, inserter);
    auto solution = construction.construct(problem);
    // 0
    // 4 0
    // 1 4 0
    // 1 2 4 0
    // 1 2 3 4 0
    EXPECT_EQ("1 2 3 4 0", solution.representation());
}

// Ensure that in construction, the selector determines the cities from the beginning.
TEST_F(ConstructionTest, InitialCitySelection)
{
    int count = 0;

    struct MockSelector
    {
        MockSelector(int& count) : count_(count) {}
        int& count_;
        Vertex select(const Problem& problem, const Solution& partialSolution)
        {
            count_++;
            switch (count_) {
            case 1:
                EXPECT_EQ("", partialSolution.representation());
                return 0;
            case 2:
                EXPECT_EQ("0", partialSolution.representation());
                return 1;
            default:
                return count_ - 1;
            }
        }
    } mockSelector(count);

    auto inserter = BestTourInserter();
    auto construction = SelectInsertConstruction(mockSelector, inserter);
    construction.construct(problem);
    EXPECT_GT(count, 2);
}
