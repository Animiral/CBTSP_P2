// tests for construction heuristics
#include "gtest/gtest.h"

import construction;

//def setUp(self) :
//    self.instance = Instance(5)
//    self.instance.add_edge(Edge(0, 1, 1))
//    self.instance.add_edge(Edge(1, 2, 2))
//    self.instance.add_edge(Edge(2, 3, 3))
//    self.instance.add_edge(Edge(3, 4, -1))
//    self.instance.add_edge(Edge(4, 0, -2))

// Ensure that vertex selection happens according to the RNG.
TEST(Construction, SelectRandom)
{
    //mock_random.choice = lambda sequence : sequence[1]
    //selector = RandomSelector(mock_random)
    //partial_solution = Solution(self.instance, [0, 2])
    //self.assertEqual(3, selector.select(self.instance, partial_solution))
}

// Ensure that the farthest vertex is always selected.
TEST(Construction, SelectFarthestCity)
{
    //selector = FarthestCitySelector()
    //self.assertEqual(0, selector.select(self.instance, Solution(self.instance, [])))
    //self.assertEqual(4, selector.select(self.instance, Solution(self.instance, [1, 2])))
    //self.assertEqual(3, selector.select(self.instance, Solution(self.instance, [0, 2])))
    //self.assertEqual(2, selector.select(self.instance, Solution(self.instance, [0, 3, 4])))
}

// Ensure that inserting places the new vertex at the best spot in the tour.
TEST(Construction, BestTourInserter)
{
    //inserter = BestTourInserter()
    //partial_solution = Solution(self.instance, [0, 2, 4])
    //next_solution = inserter.insert(self.instance, partial_solution, 3)
    //self.assertEqual([0, 2, 3, 4], next_solution.vertices)
}

// Ensure that random construction results in a full-length solution.
TEST(Construction, ConstructRandom)
{
    //selector = RandomSelector(Random())
    //inserter = BestTourInserter()
    //construction = Construction(selector, inserter)
    //solution = construction(self.instance)
    //self.assertEqual(5, len(solution.vertices))
}

// Ensure that deterministic construction results in a full-length solution.
TEST(Construction, ConstructDeterministic)
{
    //selector = FarthestCitySelector()
    //inserter = BestTourInserter()
    //construction = Construction(selector, inserter)
    //solution = construction(self.instance)
    //# 0
    //# 2 0
    //# 3 2 0
    //# 3 2 1 0
    //# 4 3 2 1 0
    //self.assertEqual([4, 3, 2, 1, 0], solution.vertices)
}

// Ensure that in construction, the selector determines the cities from the beginning.
TEST(Construction, InitialCitySelection)
{
    //count = 0

    //def select_mock(_, solution) :
    //    nonlocal count
    //    count = count + 1
    //    if count == 1 :
    //        self.assertEqual([], solution.vertices)
    //        return 0
    //        elif count == 2 :
    //        self.assertEqual([0], solution.vertices)
    //        return 1
    //    else:
    //        return count - 1

    //selector = mock.Mock()
    //selector.select = mock.Mock(side_effect = select_mock)
    //inserter = BestTourInserter()
    //construction = Construction(selector, inserter)
    //construction(self.instance)
    //self.assertGreater(count, 2)
}
