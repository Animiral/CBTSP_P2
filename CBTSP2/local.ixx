/**
 * Implementation of the local search algorithm and associated entities.
 *
 * A neighborhood is a view that derives neighbors and their tour value from a base solution.
 * A step function chooses one neighbor according to some criteria.
 */
module;

#include <iterator>
#include <memory>
#include <random>

export module local;

import cbtsp;
import construction;

/**
 * Base for neighborhood implementations.
 * 
 * A Neighborhood is a non-standard iterator that can
 * quickly compute the value of adjacent solutions.
 *
 * It can produce the indicated neighbor solution object on demand.
 * It compares equal to `std::default_sentinel` for a past-end marker.
 *
 * Neighborhoods are only defined for full, not partial, solutions.
 */
export class Neighborhood
{

public:

    /**
     * Initialize the Neighborhood for the given tour size.
     *
     * @param vertices: number of vertices in the problem
     */
    explicit Neighborhood(std::size_t vertices) noexcept;

    /**
     * Return a copy of this Neighborhood in its current state.
     */
    virtual std::unique_ptr<Neighborhood> clone() const = 0;

    /**
     * Advance to the next neighbor.
     */
    virtual Neighborhood& operator++() = 0;

    /**
     * Compute the objective value for the currently indicated neighbor.
     *
     * @param base: base solution
     * @return: the objective value of the currently indicated neighbor relative to base
     */
    virtual Value objective(const Solution& base) const noexcept = 0;

    /**
     * Change the base solution object into its neighbor according to
     * the current state of the neighborhood iterator.
     *
     * @param solution: base solution to modify
     */
    virtual void apply(Solution& solution) const = 0;

    /**
     * Produce a solution object that is the neighbor of the given base
     * solution according to the current state of the neighborhood iterator.
     */
    Solution applyCopy(const Solution& base) const;

    /**
     * Determine whether the iterator is exhausted.
     *
     * @return true if there are still neighbors available, false if the iterator is at the end.
     */
    virtual bool operator!=(std::default_sentinel_t) const noexcept = 0;

protected:

    /**
     * Stores the number of vertices in the problem.
     */
    std::size_t vertices_;

    // slicing is forbidden
    Neighborhood(const Neighborhood& rhs) = default;
    Neighborhood& operator=(const Neighborhood& ) = default;

};

/**
 * Generate neighbors from the base solution by exchanging two edges.
 *
 * This implementation supports all variants of two-edge exchange with length
 * restriction: both sub-tours between the two exchanged edges must have a
 * length that lies between a minimum length and a maximum length.
 *
 * For example, if both the minimum and the maximum length are set to 2, this
 * will only allow switching the places of two adjacent vertexes in the tour sequence.
 * This neighborhood is very local.
 */
export class TwoExchangeNeighborhood : public Neighborhood
{

public:

    /**
     * Initialize the Neighborhood for the given tour size and length restrictions.
     *
     * @param vertices: number of vertices in the problem
     * @param minl: minimum length of a subtour between edge exchange, e.g. std::max(vertices / 4, 3ull)
     * @param maxl: maximum length of the short subtour between edge exchange
     */
    explicit TwoExchangeNeighborhood(std::size_t vertices,
        std::size_t minl, std::size_t maxl) noexcept;

    /**
     * Initialize the Neighborhood for the given tour size and length restrictions.
     *
     * If the minimum length and maximum length are not specified, the default is
     * to allow any length - the same as a full two-exchange neighborhood.
     *
     * @param vertices: number of vertices in the problem
     * @param minl: minimum length of a subtour between edge exchange
     */
    explicit TwoExchangeNeighborhood(std::size_t vertices,
        std::size_t minl = 2ull) noexcept;

    //! Copy-constructor.
    TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs);

    std::unique_ptr<Neighborhood> clone() const override;
    TwoExchangeNeighborhood& operator++() override;
    Value objective(const Solution& base) const noexcept override;
    void apply(Solution& solution) const override;
    bool operator!=(std::default_sentinel_t) const noexcept override;

private:

    std::size_t minl_; // maximum number of vertices in a sub-tour
    std::size_t maxl_; // maximum number of vertices in a sub-tour
    std::size_t cut1_; // first edge to exchange is before vertex at this position
    std::size_t cut2_; // second edge to exchange is before vertex at this position

};

/**
 * Interface for step functions.
 */
export class Step
{

public:

    /**
     * Construct the base of the StepFunction for the given neighborhood.
     */
    explicit Step(std::unique_ptr<Neighborhood> neighborhood) noexcept;

    virtual ~Step() noexcept = default;

    /**
     * Modify the solution by one step according to the step rules.
     *
     * @param base: current candidate solution
     */
    virtual void step(Solution& base) = 0;

protected:

    //! the neighborhood structure for solutions
    std::unique_ptr<Neighborhood> neighborhood_;

};

/**
 * First improvement step function.
 */
export class FirstImprovement : public Step
{

public:

    /**
     * Construct the first improvement step function for the given neighborhood.
     */
    explicit FirstImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept;

    /**
     * Modify the solution to the first neighbor which offers an improvement over the base value.
     */
    virtual void step(Solution& base) override;

};


/**
 * Best improvement step function.
 */
export class BestImprovement : public Step
{

public:

    /**
     * Construct the best improvement step function for the given neighborhood.
     */
    explicit BestImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept;

    /**
     * Modify the solution to the neighbor which offers the best improvement over the base value.
     */
    virtual void step(Solution& base) override;

};


/**
 * Random step function.
 */
export template<typename Rng> class StepRandom : public Step
{

public:

    /**
     * Construct the random step function for the given neighborhood.
     *
     * @param neighborhood: walk randomly among this neighborhood
     * @param rng: standard random number generator to take random numbers from
     */
    explicit StepRandom(std::unique_ptr<Neighborhood> neighborhood, const Rng& rng) noexcept
        : Step(move(neighborhood)), rng_(rng)
    {
    }

    /**
     * Modify the solution to a random neighbor.
     */
    virtual void step(Solution& base) override
    {
        // extremely stupid count, since neighborhood does not know its own size
        int neighbors = 0;

        for (auto ns = neighborhood_->clone(); *ns != std::default_sentinel; ++ * ns)
            neighbors++;

        if (neighbors > 0) {
            const auto distribution = std::uniform_int_distribution{ 0, neighbors - 1 };
            const int choice = distribution(rng_);
            auto ns = neighborhood_->clone();

            for (int i = 0; i < choice; i++)
                ++* ns;

            ns->apply(base);
        }
    }

private:

    Rng rng_;

};

/**
 * Termination condition to be used with search.
 *
 * This condition is fulfilled when the newest solution does not
 * improve on the best solution seen so far.
 */
export class WhenStagnant
{

public:

    WhenStagnant() noexcept;

    /**
     * Evaluate the given solution against the best objective value in memory.
     *
     * @param solution: the candidate solution
     * @return: false if the solution constitutes an improvement, true if we are no longer improving
     */
    bool doneAfter(const Solution& solution) noexcept;

private:

    Value best_; //!< best value seen so far

};


/**
 * Local search meta - algorithm implementation.
 */
export class LocalSearch
{

public:

    /**
     * Construct the local search.
     *
     * @param step: step function
     * @param doTerminate: predicate to decide when to terminate the search
     */
    explicit LocalSearch(std::unique_ptr<Step> step, WhenStagnant doTerminate) noexcept;

    /**
     * Execute the search from the given start solution.
     *
     * @param solution: start solution
     * @return: the last solution found before the termination condition
     */
    Solution search(Solution solution);

private:

    std::unique_ptr<Step> step_;
    WhenStagnant doTerminate_;

};

/**
 * Standalone Local search meta - algorithm implementation.
 * This local search bundles a construction heuristic and can be applied to whole instances.
 */
export class StandaloneLocalSearch : public Search
{

public:

    /**
     * Construct the local search.
     *
     * @param construction: factory for the initial solution
     * @param step: step function
     * @param doTerminate: predicate to decide when to terminate the search
     */
    explicit StandaloneLocalSearch(
        std::unique_ptr<Construction> construction,
        std::unique_ptr<Step> step,
        WhenStagnant doTerminate) noexcept;

    /**
     * Execute the search for the given instance.
     *
     * @param problem: CBTSP instance
     * @return: the last solution found before the termination condition
     */
    virtual Solution search(const Problem& problem) override;

private:

    std::unique_ptr<Construction> construction_;
    LocalSearch local_;

};
