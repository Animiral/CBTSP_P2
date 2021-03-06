/**
 * This module implements the Mouse (Ant) Colony Optimization metaheuristic.
 */
module;

#include <vector>
#include <memory>

export module mco;

import cbtsp;
import local;

export using Pheromone = float;

/**
 * Specifies from which found solution the search heuristic reinforces pheromones.
 */
export enum class ReinforceStrategy
{
    DARWIN, //!< Reinforce based on the generated tour before improvement heuristic.
    LAMARCK //!< Reinforce based on the final tour after improvement heuristic.
};

/**
 * Models a state in the MCO search while it is underway.
 */
class McoState
{

public:

    /**
     * Construct a state for solving the given problem from the beginning.
     *
     * @param problem: problem instance
     * @param init: initial value of all pheromones
     * @param min: lowest pheromone level
     * @param max: highest pheromone level
     */
    explicit McoState(const Problem& problem, Pheromone init,
        Pheromone min, Pheromone max);

    /**
     * Look up the pheromone in the lookup vector.
     */
    Pheromone pheromone(Vertex a, Vertex b) const noexcept;

    /**
     * Spread pheromones along the solution edges based on the objective value.
     *
     * @param solution: complete solution object
     * @param scale: multiply pheromone bonus by this factor
     */
    void reinforce(const Solution& solution, float scale = 1.f) noexcept;

    /**
     * Revert pheromone intensity everywhere.
     *
     * The updated intensity is the current intensity, with a certain fraction
     * replaced by the lowest pheromone intensity.
     *
     * @param evaporation: fraction of lowest pheromone level
     */
    void evaporate(float evaporation) noexcept;

    /**
     * Apply pheromone changes everywhere.
     */
    void update() noexcept;

private:

    const Problem* problem_; //!< problem instance
    Pheromone min_; //!< lowest pheromone level
    Pheromone max_; //!< highest pheromone level
    EdgeTable<Pheromone> pheromone_; //!< current pheromone levels for every edge
    EdgeTable<Pheromone> delta_; //!< upcoming pheromone update

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
     * @param intensification: probability to outright select the best step
     * @param random: random number generator
     */
    explicit Mouse(const Problem& problem, McoState& state,
        float pheromoneAttraction, float objectiveAttraction,
        float intensification, const std::shared_ptr<Random>& random) noexcept;

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
    float intensification_; //!< probability to outright select the best step
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
     * @param elitism: factor of pheromone contribution of best solution so far
     * @param minPheromone: minimum pheromone value
     * @param maxPheromone: maximum and initial pheromone value
     * @param pheromoneAttraction: to which degree local pheromones attract
     * @param objectiveAttraction: to which degree local objective value attracts
     * @param intensification: chance of choosing best step
     * @param reinforceStrategy: from which found solution to reinforce pheromones
     * @param random: random number generator
     * @param improvement: improvement heuristic to apply after Mouse construction
     */
    explicit Mco(int ticks, int mice, float evaporation, float elitism,
        float minPheromone, float maxPheromone,
        float pheromoneAttraction, float objectiveAttraction,
        float intensification, ReinforceStrategy reinforceStrategy,
        const std::shared_ptr<Random>& random, std::unique_ptr<LocalSearch> improvement) noexcept;

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
    float elitism_; // factor of pheromone contribution of best solution so far
    float minPheromone_; //!< minimum pheromone value
    float maxPheromone_; //!< maximum and initial pheromone value
    float pheromoneAttraction_; // to which degree local pheromones attract
    float objectiveAttraction_; // to which degree local objective value attracts
    float intensification_; // chance of choosing best step
    ReinforceStrategy reinforceStrategy_; // from which found solution to reinforce pheromones
    std::shared_ptr<Random> random_; //!< random number generator
    std::unique_ptr<LocalSearch> improvement_; //!< improvement heuristic

};
