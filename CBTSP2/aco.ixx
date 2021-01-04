/**
 * This module implements the Ant Colony Optimization metaheuristic.
 */
module;

#include <utility>

export module aco;

import cbtsp;

/**
 * ACO implementation.
 */
export class Aco : public Search
{

public:

    /**
     * Construct the search.
     */
    explicit Aco() noexcept
    {
    }

    /**
     * Execute the ACO scheme for the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the solution
     */
    virtual Solution search(const Problem& problem) override
    {
        std::vector<Vertex> vs;
        return Solution{ problem, move(vs) }; // TODO: implement
    }

};
