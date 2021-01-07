/**
 * This module defines our construction heuristics.
 */
module;

#include <random>
#include <utility>

export module construction;

export import cbtsp;

/**
 * Calculate the set of vertices in the problem which are available
 * to the selection strategy, i.e. all those not yet part of the solution.
 */
std::vector<Vertex> selectables(const Problem& problem, const Solution& partialSolution);

/**
 * Random selection strategy.
 *
 * The next vertex which is not part of the solution is chosen at random according to the given RNG.
 */
export template<typename Rng> class RandomSelector
{

public:

    /**
     * Construct a random selection strategy that takes random numbers from the given generator.
     */
    explicit RandomSelector(const Rng& rng)
        : rng_(rng)
    {
    }

    /**
     * Choose any random next vertex from the instance which is not yet part of the solution.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object
     * @return: random next vertex
     */
    Vertex select(const Problem& problem, const Solution& partialSolution)
    {
        const auto selectable = selectables(problem, partialSolution);
        const auto distribution = std::uniform_int_distribution{ 0ull, selectable.size() - 1 };
        return selectable[distribution(rng_)];
    }

private:

    Rng rng_;

};

/**
 * Farthest city selection strategy.
 */
export class FarthestCitySelector
{

public:

    /**
     * Choose the vertex whose minimum absolute value to any existing
     * partial - solution vertex is maximum.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object
     * @return: farthest next vertex
     */
    Vertex select(const Problem& problem, const Solution& partialSolution);

};


/**
 * Vertex insertion strategy.
 *
 * The new vertex is placed in the tour to minimize the absolute intermediate tour value.
 */
export class BestTourInserter
{

public:

    /**
     * Add the next vertex to the partial solution in the best - value place.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object to modify
     * @param nextVertex: vertex number
     */
    void insert(const Problem& problem, Solution& partialSolution, Vertex nextVertex) const;

private:

    /**
     * Determine the value of the tour which results from inserting the next vertex at the specified spot in the tour.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object, state restored after experiment
     * @param nextVertex: vertex number
     * @param pos: index before which to insert the next vertex in the tour
     * @return: the value of the resulting tour
     */
    static Value tourObjective(const Problem& problem, Solution& partialSolution, Vertex nextVertex, std::size_t pos);

};

/**
 * A Construction is a heuristic that can create an initial full -
 * not necessarily feasible or optimal - solution for a given problem instance.
 */
export class Construction
{

public:

    virtual ~Construction() noexcept = default;

    /**
     * Construct a solution using the heuristic implementation.
     *
     * @param problem: problem instance object
     * @return: a full - length Solution object
     */
    virtual Solution construct(const Problem& problem) = 0;

};

/**
 * Constructs a full - length CBTSP solution based on a selection strategy and an insertion strategy.
 *
 * The selection strategy specifies the next vertex to be added to the tour.
 * The insertion strategy defines where the next vertex should be inserted in the partial solution.
 */
export template<typename SelectionStrategy> class SelectInsertConstruction : public Construction
{

public:

    /**
    * Constructor.
    * 
    * @param selector: selection strategy
    * @param inserter: insertion strategy
     */
    explicit SelectInsertConstruction(SelectionStrategy selector, BestTourInserter inserter) noexcept
        : Construction(), selector_(selector), inserter_(inserter)
    {
    }

    /**
     * Construct a solution using the configured strategies.
     *
     * @param problem: problem instance object
     * @return: a full - length Solution object
     */
    Solution construct(const Problem& problem) override
    {
        auto solution = Solution(problem, {});
        for (std::size_t i = 0; i < problem.vertices(); i++) {
            const Vertex next = selector_.select(problem, solution);
            inserter_.insert(problem, solution, next);
        }

        return solution;
    }

private:

    SelectionStrategy selector_;
    BestTourInserter inserter_;

};

export using RandomConstruction = SelectInsertConstruction<RandomSelector<std::default_random_engine>>;
export using DeterministicConstruction = SelectInsertConstruction<FarthestCitySelector>;
