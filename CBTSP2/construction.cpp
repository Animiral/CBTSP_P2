module;

#include <cstddef>
#include <utility>

module construction;

Vertex FarthestCitySelector::select(const Problem& problem, const Solution& partialSolution)
{
    return 0; // TODO: translate Python
    //max_vertex = 0
    //max_distance = -1

    //for vertex in range(instance.vertices) :
    //    if vertex in partial_solution.vertices : # already selected
    //        continue

    //    distances = (instance.value(vertex, other) for other in partial_solution.vertices)
    //    distance = min((abs(d) for d in distances), default = 0)  # min distance to connect to existing solution

    //    if distance > max_distance:
    //        max_vertex = vertex
    //        max_distance = distance

    //return max_vertex
}

Solution BestTourInserter::insert(const Problem& problem, const Solution& partialSolution, Vertex nextVertex) const
{
    std::vector<Vertex> vs;
    return Solution{ problem, move(vs) }; // TODO: translate Python
    //if not partial_solution.vertices :
    //    return Solution(instance, [next_vertex])

    //candidate_values = (abs(self.__tour_value(instance, partial_solution, next_vertex, i))
    //for i in range(len(partial_solution.vertices)))
    //    best_i, best_v = min(enumerate(candidate_values), key = lambda iv : abs(iv[1]))
    //    vl = partial_solution.vertices
    //    return Solution(instance, vl[0:best_i] + [next_vertex] + vl[best_i:])
}

Value BestTourInserter::tourValue(const Problem& problem, const Solution& partialSolution, Vertex nextVertex, std::size_t where)
{
    return 0; // TODO: translate Python
    //vl = partial_solution.vertices
    //predecessor = vl[where - 1]
    //successor = vl[where]
    //return partial_solution.value - instance.value(predecessor, successor) + \
        //instance.value(predecessor, next_vertex) + instance.value(next_vertex, successor)
}
