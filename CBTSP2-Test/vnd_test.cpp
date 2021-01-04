// tests for the VND implementation
#include "gtest/gtest.h"

import vnd;

// Test the basic functionality of Vnd.
TEST(Vnd, BasicRun)
{
    //Our instance is an array of numbers. Solutions are array indices.
    //Local search consists of moving to the highest adjacent value.
    //Wide search consists of moving two values along.
    //VND must be able to locate and then escape the local optimum.
    //"""
    //instance = [0, 1, 0, 10, 15, 0]  # 15 is the maximum

    //def objective(i):
    //    nonlocal instance
    //    return -instance[i % len(instance)]

    //def local_search(start):
    //    nonlocal objective
    //    return min(start - 1, start, start + 1, key=objective)

    //def wide_search(start):
    //    nonlocal objective
    //    return min(start - 2, start, start + 2, key=objective)

    //vnd = Vnd(lambda _: 0, [local_search, wide_search], objective)
    //self.assertEqual(4, vnd(instance))
}
