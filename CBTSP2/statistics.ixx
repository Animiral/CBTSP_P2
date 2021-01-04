/**
 * This module contains definitions for run statistics which we collect for output.
 */
module;

#include <optional>

export module statistics;

import cbtsp;

/**
 * Keeps records about the solutions which are produced by the algorithms.
 */
export class Statistics
{

public:

    Statistics() noexcept;

    /**
     * Enter the given solution into the statistics record.
     */
    void submit(const Solution& solution);

    /**
     * Get the best solution which was ever submitted to the Statistics.
     */
    const Solution* bestSolution() const noexcept;

    /**
     * Get the number of submitted solutions which were infeasible(containing non - edges)
     */
    int countInfeasible() const noexcept;

    /**
     * Get the total number of Solutions submitted.
     */
    int countTotal() const noexcept;

private:

    std::optional<Solution> bestSolution_;
    int countInfeasible_;
    int countTotal_;

};
