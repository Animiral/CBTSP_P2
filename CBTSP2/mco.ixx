/**
 * This module implements the Mouse (Ant) Colony Optimization metaheuristic.
 */
module;

#include <vector>
#include <memory>

export module mco;

import cbtsp;

using Pheromone = float;

/**
 * Models a state in the MCO search while it is underway.
 */
class McoState
{

public:

    /**
     * Construct a state for solving the given problem from the beginning.
     */
    explicit McoState(const Problem& problem);

    /**
     * Look up the pheromone in the lookup vector.
     */
    const Pheromone& pheromone(Vertex a, Vertex b) const noexcept;

    /**
     * Add pheromone value to an edge to be applied at the next update.
     */
    void reinforce(Vertex a, Vertex b, Pheromone delta) noexcept;

    /**
     * Lower pheromone intensity everywhere.
     */
    void evaporate(float evaporation) noexcept;

    /**
     * Apply pheromone changes everywhere.
     */
    void update() noexcept;

private:

    std::size_t vertices_; //!< problem size
    std::vector<Pheromone> pheromone_; //!< current pheromone levels for every edge
    std::vector<Pheromone> delta_; //!< upcoming pheromone update

};

/**
 * Models the behavior of a mouse in the colony.
 */
class Mouse
{

public:

    /**
     * Construct a Mouse to traverse the given search state.
     *
     * @param problem: CBTSP problem instance
     * @param state: search state with pheromone info
     * @param pheromoneAttraction: to which degree local pheromones attract
     * @param objectiveAttraction: to which degree local objective value attracts
     */
    explicit Mouse(const Problem& problem, McoState& state,
        float pheromoneAttraction, float objectiveAttraction,
        const std::shared_ptr<Random>& random) noexcept;

    /**
     * Traverse the problem to construct a solution.
     * Based on the final value of the solution, paths will be reinforced with a pheromone bonus.
     *
     * @return: the generated solution
     */
    Solution construct();

private:

    /**
     * For the given partially traversed solution, use pheromone and projected
     * visible benefit information to decide on the next Vertex step.
     *
     * @param solution: partially traversed solution object
     */
    std::size_t decideNext(const Solution& solution, std::size_t position);

    const Problem* problem_; //!< problem info
    McoState* state_; //!< pheromone info
    float pheromoneAttraction_; //!< pheromone attraction
    float objectiveAttraction_; //!< objective attraction
    std::shared_ptr<Random> random_; //!< random number generator

};

/**
 * Mouse Colony Optimization implementation.
 *
 * This is a very basic Mouse Colony search heuristic.
 * It runs until it cannot find any improvement to the solution within a given time.
 */
export class Mco : public Search
{

public:

    /**
     * Construct the search.
     *
     * @param ticks: number of iterations on a stagnated solution before termination
     * @param mice: number of traversals within a tick to construct solution candidates
     * @param evaporation: fraction of pheromone decrease per tick
     * @param pheromoneAttraction: to which degree local pheromones attract
     * @param objectiveAttraction: to which degree local objective value attracts
     * @param random: random number generator
     */
    explicit Mco(int ticks, int mice, float evaporation,
        float pheromoneAttraction, float objectiveAttraction,
        const std::shared_ptr<Random>& random) noexcept;

    /**
     * Execute the MCO scheme for the given problem.
     *
     * @param problem: CBTSP instance
     * @return: the solution
     */
    virtual Solution search(const Problem& problem) override;

private:

    int ticks_; // number of iterations on a stagnated solution before termination
    int mice_; // number of traversals within a tick to construct solution candidates
    float evaporation_; // fraction of pheromone decrease per tick
    float pheromoneAttraction_; // to which degree local pheromones attract
    float objectiveAttraction_; // to which degree local objective value attracts
    std::shared_ptr<Random> random_; //!< random number generator

};