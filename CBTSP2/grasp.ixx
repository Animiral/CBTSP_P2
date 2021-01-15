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
 * GRASP implementation.
 */
export class Grasp : public Search
{

public:

    /**
     * Construct the search.
     *
     * @param construction: construction heuristic, must be randomized
     * @param improvement: improvement heuristic a.k.a. local search
     * @param iterations: number of random constructions to consider
     */
    explicit Grasp(std::unique_ptr<Construction> construction,
        std::unique_ptr<LocalSearch> improvement, int iterations) noexcept;

    /**
     * Execute the GRASP search scheme for the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the best solution found within the iterations
     */
    virtual Solution search(const Problem& problem) override;

private:

    std::unique_ptr<Construction> construction_;
    std::unique_ptr<LocalSearch> improvement_;
    int iterations_;

};
