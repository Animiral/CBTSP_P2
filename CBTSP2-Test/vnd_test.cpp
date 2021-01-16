// tests for the VND implementation
#include "gtest/gtest.h"

#include <vector>
#include <memory>
#include <utility>

import vnd;
import construction;
import cbtsp;
import local;

// Test the basic functionality of Vnd.
TEST(Vnd, BasicRun)
{
    // We provide a problem instance with one locally optimal (l.o.) solution
    // and one globally optimal (g.o.) solution for the test.
    //
    //   /-- 1 --\
    //  /         \
    // 0           2
    // |\_________/|
    // |/         \|
    // 6          -3
    // |\_________/|
    // |/         \|
    // 5 ----------4
    //
    // The global optimum is { 0, 1, 2, 3, 4, 5, 6 }.
    // Search starts from { 0, 1, 2, 6, 5, 4, 3 }, using the X-crossing in the middle.
    //
    // We use vertex-shift as the narrow neighborhood, which prevents the VND
    // from considering tearing up the suboptimal {2, 6} and {0, 3} edges.
    //
    // Instead, it first finds the local optimum, then switches to wide search
    // and finally comes around to the global optimum.

    auto problem = Problem{ 7, 100l };
    problem.addEdge({ 0, 1, 0 });
    problem.addEdge({ 1, 2, 0 });
    problem.addEdge({ 2, 3, 0 });
    problem.addEdge({ 3, 4, 0 });
    problem.addEdge({ 4, 5, 0 });
    problem.addEdge({ 5, 6, 0 });
    problem.addEdge({ 6, 0, 0 });
    problem.addEdge({ 2, 6, 9 }); // bad edge #1
    problem.addEdge({ 0, 3, 9 }); // bad edge #2
    problem.addEdge({ 3, 5, -1 }); // slightly bad for local optimum
    problem.addEdge({ 4, 6, -1 }); // slightly bad for local optimum

    struct MockConstruction : public Construction
    {
        Solution construct(const Problem& problem) override
        {
            return Solution{ problem, { 0, 1, 2, 6, 5, 4, 3 } };
        }
    };
    auto construction = std::make_unique<MockConstruction>();
    auto narrow = std::make_unique<FirstImprovement>(std::make_unique<TwoExchangeNeighborhood>(2, 2));
    auto wide = std::make_unique<FirstImprovement>(std::make_unique<WideNeighborhood>());
    auto steps = std::vector<std::unique_ptr<Step>>();
    steps.push_back(move(narrow));
    steps.push_back(move(wide));

    auto vnd = Vnd(move(construction), move(steps));
    auto actual = vnd.search(problem);
    actual.normalize();
    const auto optimum = Solution{ problem, { 0, 1, 2, 3, 4, 5, 6 } };

    EXPECT_EQ(optimum.vertices(), actual.vertices());
}
