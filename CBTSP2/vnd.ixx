/**
 * This module implements the Variable Neighborhood Descent heuristic.
 */
module;

#include <vector>
#include <memory>

export module vnd;

import cbtsp;
import construction;
import local;

/**
 * Variable Neighborhood Descent heuristic.
 */
export class Vnd : public Search
{

public:

    /**
     * Construct the VND search.
     *
     * @param construction: construction heuristic for getting the initial solution from the instance
     * @param steps: step functions across neighborhood structures, from local to wide
     */
    explicit Vnd(std::unique_ptr<Construction> construction,
        std::vector<std::unique_ptr<Step>> steps) noexcept;

    /**
     * Run the VND algorithm on the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the best solution found regarding all neighborhood structures
     */
    virtual Solution search(const Problem& problem) override;

private:

    std::unique_ptr<Construction> construction_;
    std::vector<std::unique_ptr<Step>> steps_;

};
