/**
 * Implementation of the GRASP (Greedy Randomized Adaptive Search Procedure) meta-heuristic.
 *
 * A Randomized Construction Heuristic is a callable entity which returns a full solution.
 * A Local Search Algorithm is a callable entity which, given a solution,
 * returns a feasible solution of some reasonable quality (local optimum).
 * These building blocks are defined in the separate "local" module.
 */
module;

#include <memory>

export module grasp;

import cbtsp;
import construction;
import local;

/**
 * Termination condition to be used with GRASP.
 */
export class AfterIterations
{

public:

    /**
     * Construct the condition with the specified number of iterations.
     *
     * @param iterations: number of times to indicate non-termination
     */
    explicit AfterIterations(int iterations) noexcept;

    /**
     * Return whether the specified number of iterations(assuming one call per iteration) have passed.
     *
     * @return: true if the configured number of iterations has passed, false otherwise
     */
    bool done() noexcept;

private:

    int iterations_;

};

/**
 * GRASP implementation.
 */
export class Grasp : public Search
{

public:

    /**
     * Construct the search.
     *
     * @param construction: construction heuristic, must be randomized
     * @param localSearch: local search algorithm
     * @param doTerminate: predicate to decide when to terminate the search
     */
    explicit Grasp(std::unique_ptr<Construction> construction, LocalSearch&& localSearch, AfterIterations doTerminate) noexcept;

    /**
     * Execute the GRASP search scheme for the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the last solution found before the termination condition
     */
    virtual Solution search(const Problem& problem) override;

private:

    std::unique_ptr<Construction> construction_;
    LocalSearch localSearch_;
    AfterIterations doTerminate_;

};
