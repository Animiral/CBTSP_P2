// tests for the GRASP implementation
#include "gtest/gtest.h"
#include <cassert>

import grasp;
import cbtsp;
import construction;
import local;

// Test the basic functionality of Grasp.
TEST(Grasp, BasicRun)
{
    // We provide a problem instance with one locally optimal (l.o.) solution
    // and one globally optimal (g.o.) solution for the test.
    //
    // The injected construction will let GRASP find the solutions in order:
    // l.o., g.o., l.o.
    //
    // From this result, the algorithm must finally pick and return g.o.
    //
    //  /--- 1 ---\
    // 0           2
    // |\---- ----/|
    // |     X     |
    // |/---- ----\|
    // 5           3
    //  \--- 4 ---/

    auto problem = Problem{ 6, 100l };
    problem.addEdge({ 0, 1, 1 });
    problem.addEdge({ 1, 2, -1 });
    problem.addEdge({ 2, 3, 0 });
    problem.addEdge({ 5, 0, 0 });
    problem.addEdge({ 2, 5, 1 });
    problem.addEdge({ 0, 3, 1 });
    problem.addEdge({ 3, 4, 1 });
    problem.addEdge({ 4, 5, -1 });

    const auto localOpt = Solution{ problem, {0, 1, 2, 5, 4, 3} };
    const auto globalOpt = Solution{ problem, {0, 1, 2, 3, 4, 5} };

    const Solution inits[] = {
        Solution{problem, {1, 0, 2, 5, 4, 3}},
        Solution{problem, {0, 1, 2, 3, 5, 4}},
        Solution{problem, {0, 1, 5, 2, 4, 3}}
    };

    struct MockConstruction : public Construction
    {
        size_t i_ = 0;
        const Solution* inits_;
        Solution construct(const Problem&) override
        {
            assert(i_ < 3);
            return inits_[i_++];
        }
    };
    auto mockConstruction = std::make_unique<MockConstruction>();
    mockConstruction->inits_ = inits;

    auto neighborhood = std::make_unique<TwoExchangeNeighborhood>(6, 2, 2);
    auto step = std::make_unique<BestImprovement>(move(neighborhood));
    auto localSearch = std::make_unique<LocalSearch>(move(step));

    auto grasp = Grasp(move(mockConstruction), move(localSearch), 3);
    Solution actual = grasp.search(problem);
    actual.normalize();
    EXPECT_EQ(actual.vertices(), globalOpt.vertices());
}
