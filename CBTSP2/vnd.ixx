/**
 * This module implements the Variable Neighborhood Descent heuristic.
 */
module;

#include <utility>

export module vnd;

import cbtsp;

/**
 * Variable Neighborhood Descent heuristic.
 */
export template<class Construction, class Steps> class Vnd : public Search
{

public:

    /**
     * Construct the VND search.
     *
     * @param construction: construction heuristic for getting the initial solution from the instance
     * @param steps: random access view of step functions across neighborhood structures, from local to wide
     */
    explicit Vnd(Construction&& construction, Steps&& steps) noexcept
        : construction_(std::move(construction)), steps_(std::move(steps))
    {
    }

    /**
     * Run the VND algorithm on the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the best solution found regarding all neighborhood structures
     */
    virtual Solution search(const Problem& problem) override
    {
        return {}; // TODO: translate Python
        //best_solution = self.construction(instance)
        //    level = 0  # begin with most local neighborhood

        //    while level < len(self.steps) :
        //        step = self.steps[level]
        //        candidate = step(best_solution)

        //        if self.objective(candidate) < self.objective(best_solution) :
        //            best_solution = candidate
        //            level = 0
        //        else:
        //            level += 1  # widen search

        //return best_solution
    }

private:

    Construction&& construction_;
    Steps&& steps_;

};
