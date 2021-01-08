/**
 * This module contains definitions for run statistics which we collect for output.
 */
module;

#include <chrono>
#include <string>

export module statistics;

import cbtsp;

using Clock = std::chrono::high_resolution_clock;
using Runtime = Clock::duration;

/**
 * Keeps records about the solutions which are produced by the algorithms.
 *
 * The following facts are provided from the records:
 *  - Number of records/samples
 *  - Number of feasible solutions found
 *  - Best found solution
 *  - Mean and standard deviation over the objective values of the feasible solutions
 *  - Mean and standard deviation over the number of infeasible edges of the infeasible solutions
 *  - Median runtime
 */
export class Statistics
{

public:

    /**
     * Construct the statistics record under the given name.
     * The name can be used later to identify the measurement.
     *
     * @param name: identifying name of the statistical record
     */
    explicit Statistics(const std::string& name);

    /**
     * Record a result.
     */
    void record(const Solution& solution, Runtime runtime);

    /**
     * Get the identifying name of the statistical record.
     */
    const std::string& name() const noexcept;

    /**
     * Get the total number of Solutions recorded.
     */
    int samples() const noexcept;

    /**
     * Get the total number of feasible Solutions recorded.
     */
    int feasibles() const noexcept;

    /**
     * Get the best Solution which was recorded in the Statistics.
     */
    const Solution* bestSolution() const noexcept;

    /**
     * Get the mean objective value of the feasible solutions.
     */
    float meanObjective() const noexcept;

    /**
     * Get the standard deviation over the objective values of the feasible solutions.
     */
    float stdevObjective() const noexcept;

    /**
     * Get the mean number of infeasible edges among infeasible solutions.
     */
    float meanInfEdges() const noexcept;

    /**
     * Get the standard deviation over the number of infeasible edges among infeasible solutions.
     */
    float stdevInfEdges() const noexcept;

    /**
     * Get the median of recorded runtimes.
     */
    Runtime medRuntime() const noexcept;

    /**
     * Execute the given search on the given problem instance repeatedly
     * and enter relevant results into the statistical record.
     *
     * @param name: identifying name of the statistical record
     * @param search: search heuristic
     * @param problem: problem instance
     * @param samples: number of repetitions to sample
     */
    static Statistics measure(const std::string& name, Search& search, const Problem& problem, int samples);

private:

    std::string name_;
    std::vector<Solution> solutions_;
    std::vector<Runtime> runtimes_;

};
