// tests for Local Search functions
#include "gtest/gtest.h"

import local;

//def setUp(self) :
//    self.instance = Instance(5)
//    self.instance.add_edge(Edge(0, 1, 1))
//    self.instance.add_edge(Edge(1, 2, -1))
//    self.instance.add_edge(Edge(2, 3, 3))
//    self.instance.add_edge(Edge(3, 4, -1))
//    self.instance.add_edge(Edge(4, 0, -2))
//    self.instance.add_edge(Edge(0, 3, 3))
//    self.instance.add_edge(Edge(1, 4, -4))

TEST(Local, TwoExchangeNeighborhood)
{
    //base_solution = Solution(self.instance, [0, 1, 2, 3, 4])
    //expected = [[0, 1, 2, 4, 3], [0, 1, 3, 2, 4], [0, 1, 4, 3, 2], [0, 2, 1, 3, 4], [0, 3, 2, 1, 4]]
    //actual = sorted(s.normalized().vertices for s in two_exchange_neighborhood(base_solution))
    //self.assertEqual(expected, actual)
}

TEST(Local, VertexShiftNeighborhood)
{
    //base_solution = Solution(self.instance, [0, 1, 2, 3, 4])
    //expected = [[0, 1, 2, 4, 3], [0, 1, 3, 2, 4], [0, 1, 4, 3, 2], [0, 2, 1, 3, 4], [0, 3, 2, 1, 4]]
    //actual = sorted(s.normalized().vertices for s in vertex_shift_neighborhood(base_solution))
    //self.assertEqual(expected, actual)
}

TEST(Local, Search)
{
    //optimum = Solution(self.instance, [0, 1, 2, 3, 4])
    //start = Solution(self.instance, [0, 1, 3, 4, 2])  # 2 steps from optimum
    //objective = Solution.objective

    //first_improvement = FirstImprovement(two_exchange_neighborhood, objective)
    //done = WhenStagnant(objective)
    //search = LocalSearch(first_improvement, done)
    //actual = search(start).normalized()
    //self.assertEqual(optimum.vertices, actual.vertices)

    //best_improvement = BestImprovement(two_exchange_neighborhood, objective)
    //done = WhenStagnant(objective)
    //search = LocalSearch(best_improvement, done)
    //actual = search(start).normalized()
    //self.assertEqual(optimum.vertices, actual.vertices)
}
