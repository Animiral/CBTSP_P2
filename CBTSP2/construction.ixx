/**
 * This module defines our construction heuristics.
 */
module;

#include <random>
#include <utility>

export module construction;

export import cbtsp;

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
        return 0; // TODO: translate Python
        //candidates = [v for v in range(instance.vertices) if v not in partial_solution.vertices]
        //return self.random.choice(candidates)
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
     * Create a new solution from the partial solution by adding the next vertex in the best - value place.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object
     * @param nextVertex: vertex number
     * @return: a Solution object with a tour including the next vertex
     */
    Solution insert(const Problem& problem, const Solution& partialSolution, Vertex nextVertex) const;

private:

    /**
     * Determine the value of the tour which results from inserting the next vertex at the specified spot in the tour.
     *
     * @param problem: problem instance object
     * @param partialSolution: partial Solution object
     * @param nextVertex: vertex number
     * @param where: index before which to insert the next vertex in the tour
     * @return: the value of the resulting tour
     */
    Value tourValue(const Problem& problem, const Solution& partialSolution, Vertex nextVertex, std::size_t where);

};

/**
 *
 * Constructs a full - length CBTSP solution based on a selection strategy and an insertion strategy.
 *
 * The selection strategy specifies the next vertex to be added to the tour.
 * The insertion strategy defines where the next vertex should be inserted in the partial solution.
 */
export template<typename SelectionStrategy> class Construction
{

public:

    /**
    * Constructor.
    * 
    * @param selector: selection strategy
    * @param inserter: insertion strategy
     */
    explicit Construction(SelectionStrategy&& selector, BestTourInserter&& inserter) noexcept
        : selector_(selector), inserter_(inserter)
    {
    }

    /**
     * Construct a solution using the configured strategies.
     *
     * @param problem: problem instance object
     * @return: a full - length Solution object
     */
    Solution construct(const Problem& problem)
    {
        std::vector<Vertex> vs;
        return Solution{ problem, move(vs) }; // TODO: translate Python
        //partial_solution = Solution(instance, [])
        //for i in range(instance.vertices) :
        //    next_vertex = self.selector.select(instance, partial_solution)
        //    partial_solution = self.inserter.insert(instance, partial_solution, next_vertex)

        //return partial_solution
    }

private:

    SelectionStrategy selector_;
    BestTourInserter inserter_;

};

export using RandomConstruction = Construction<RandomSelector<std::default_random_engine>>;
export using DeterministicConstruction = Construction<FarthestCitySelector>;
