/**
 * Implementation of the local search algorithm and associated entities.
 *
 * A neighborhood is a view that derives neighbors from a base solution.
 * A step function chooses one neighbor according to some criteria.
 */
module;

#include <iterator>
#include <memory>

export module local;

import cbtsp;
import construction;

/**
 * Base for neighborhood implementations.
 * 
 * Neighborhoods are iterators that can be combined with `std::default_sentinel`
 * in a `std::span` to obtain a view over all the neighbors.
 */
class Neighborhood
{

public:

    using value_type = Solution; //!< This iterator iterates over Solution objects.
    using reference = const value_type&; //!< Dereferencing this iterator yields references to Solutions.
    using pointer = const value_type*; //!< Defines a pointer to the iterated-over Solutions.

    /**
     * Initialize the Neighborhood from the base solution.
     *
     * @param base: initial solution
     */
    explicit Neighborhood(Solution&& base) noexcept;

    /**
     * Copy constructor.
     */
    explicit Neighborhood(const Neighborhood& rhs);

    /**
     * Advance to the next neighbor.
     */
    virtual Neighborhood& operator++() = 0;

    //! Dereference.
    reference operator*() const noexcept;

    //! Access.
    pointer operator->() const noexcept;

    /**
     * Determine whether the iterator is exhausted.
     *
     * @return true if there are still neighbors available, false if the iterator is at the end.
     */
    virtual bool operator!=(std::default_sentinel_t) const noexcept = 0;

protected:

    /**
     * Stores the object currently indicated by this iterator.
     */
    Solution solution_;

};

/**
 * Generate neighbors from the base solution by exchanging two edges.
 */
class TwoExchangeNeighborhood : public Neighborhood
{

public:

    /**
     * Initialize the iterator from the base solution.
     *
     * @param base: initial solution
     */
    explicit TwoExchangeNeighborhood(Solution&& base) noexcept;

    //! Copy-constructor.
    explicit TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs);

    TwoExchangeNeighborhood& operator++() override;

private:



};

/**
 * Generate neighbors from the base solution by exchanging two edges.
 * In this variation, the two edges must be at least 3 vertices apart,
 * but still within 1 / 4 of the instance's number of vertices of each other.
 */
class NarrowTwoExchangeNeighborhood : public Neighborhood
{

public:

    /**
     * Initialize the iterator from the base solution.
     *
     * @param base: initial solution
     */
    explicit NarrowTwoExchangeNeighborhood(Solution&& base) noexcept;

    //! Copy-constructor.
    explicit NarrowTwoExchangeNeighborhood(const NarrowTwoExchangeNeighborhood& rhs);

    NarrowTwoExchangeNeighborhood& operator++() override;

private:




};

/**
 * Generate neighbors from the base solution by exchanging two edges.
 * In this variation, the two edges must be at least 1 / 4 of the
 * instance's number of vertices apart, leading to a drastic change.
 */
class WideTwoExchangeNeighborhood : public Neighborhood
{

public:

    /**
     * Initialize the iterator from the base solution.
     *
     * @param base: initial solution
     */
    explicit WideTwoExchangeNeighborhood(Solution&& base) noexcept;

    //! Copy-constructor.
    explicit WideTwoExchangeNeighborhood(const WideTwoExchangeNeighborhood& rhs);

    WideTwoExchangeNeighborhood& operator++() override;

private:



};

/**
 * Generate neighbors from the base solution by switching the places
 * of two adjacent vertexes in the tour sequence.
 *
 * This neighborhood is very local.
 */
class VertexShiftNeighborhood : public Neighborhood
{

public:

    /**
     * Initialize the iterator from the base solution.
     *
     * @param base: initial solution
     */
    explicit VertexShiftNeighborhood(Solution&& base) noexcept;

    //! Copy-constructor.
    explicit VertexShiftNeighborhood(const VertexShiftNeighborhood& rhs);

    VertexShiftNeighborhood& operator++() override;

private:



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
export class StepRandom : public Step
{

public:

    /**
     * Construct the random step function for the given neighborhood.
     */
    explicit StepRandom(std::unique_ptr<Neighborhood> neighborhood) noexcept;

    /**
     * Modify the solution to a random neighbor.
     */
    virtual void step(Solution& base) override;

};

/**
 * Termination condition to be used with search.
 *
 * This condition is fulfilled when the newest solution does not
 * improve on the best solution seen so far.
 */
class WhenStagnant
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
     * Execute the search from the given base solution.
     *
     * @param start: base solution
     * @return: the last solution found before the termination condition
     */
    Solution search(Solution&& start) const;

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
        DeterministicConstruction&& construction,
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

    DeterministicConstruction construction_;
    LocalSearch local_;

};
