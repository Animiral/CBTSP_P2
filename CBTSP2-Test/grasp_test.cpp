// tests for the GRASP implementation
#include "gtest/gtest.h"

import grasp;

// Test the basic functionality of Grasp.
TEST(Grasp, BasicRun)
{
    //Our instance is an array of numbers.Solutions are array indices.
    //Local search consists of moving to the highest adjacent value.
    //"""
    //instance = [0, 1, 0, 10, 0, 0]  # 10 is the maximum
    //start_solutions = (i for i in[0, 2, 4])

    //def objective(i) :
    //nonlocal instance
    //return -instance[i]

    //def search(start) :
    //nonlocal objective
    //return min(start - 1, start, start + 1, key = objective)

    //done = WhenStagnant(objective)
    //grasp = Grasp(lambda _ : next(start_solutions), search, objective, done)
    //self.assertEqual(3, grasp(instance))
}

// Test the termination condition after N iterations.
TEST(Grasp, Termination)
{
    //done = AfterIterations(3)
    //self.assertEqual(False, done(None, None))
    //self.assertEqual(False, done(None, None))
    //self.assertEqual(False, done(None, None))
    //self.assertEqual(True, done(None, None))
}
