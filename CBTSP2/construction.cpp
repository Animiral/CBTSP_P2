module;

#include <cstddef>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

module construction;

std::vector<Vertex> selectables(const Problem& problem, const Solution& partialSolution)
{
    std::vector<Vertex> all(problem.vertices());
    std::iota(all.begin(), all.end(), 0);
    std::vector<Vertex> unselectable = partialSolution.vertices();
    std::ranges::sort(unselectable);

    // get just the selectables
    std::vector<Vertex> selectable(problem.vertices() - partialSolution.length());
    std::ranges::set_difference(all, unselectable, selectable.begin());

    return selectable;
}

Vertex FarthestCitySelector::select(const Problem& problem, const Solution& partialSolution)
{
    if (0 == partialSolution.length())
        return 0; // start from vertex 0

    const auto selectable = selectables(problem, partialSolution);

    // Function to evaluate distance to specific selection candidates
    struct DistanceTo
    {
        const Problem& problem;
        Vertex fixpoint; // vertex to measure distance to
        Value operator()(Vertex vertex) const
        {
            return std::abs(problem.value(fixpoint, vertex));
        }
    };

    // Function to find out the min distance from a candidate to the partial solution
    using DVPair = std::pair<Value, Vertex>;
    auto getMinDistance = [&problem, &partialSolution](Vertex candidate) -> DVPair
    {
        const auto distanceTo = DistanceTo{ problem, candidate };
        const auto distances = std::ranges::transform_view(partialSolution.vertices(), distanceTo);
        const Value minDistance = *std::ranges::min_element(distances);
        return std::make_pair(minDistance, candidate);
    };

    // Find all selection candidates paired with their evaluated distance
    const auto evaluation = std::ranges::transform_view(selectable, getMinDistance);

    // return the farthest candidate
    const Value bigM = problem.bigM();
    const auto greaterDistance = [bigM](DVPair dv1, DVPair dv2)
    {
        // preferably avoid big-M edges in the farthest that we're looking for
        return (dv1.first != bigM) && dv1.first > dv2.first;
    };
    return (*std::ranges::min_element(evaluation, greaterDistance)).second;
}

void BestTourInserter::insert(const Problem& problem, Solution& partialSolution, Vertex nextVertex) const
{
    // get a list of all possible inserts
    auto evaluation = std::vector<Value>(partialSolution.length());
    for (std::size_t i = 0; i < partialSolution.length(); i++) {
        evaluation[i] = tourObjective(problem, partialSolution, nextVertex, i);
    }

    // find the best one
    const auto minAt = std::min_element(evaluation.begin(), evaluation.end());

    // apply next vertex at best position
    partialSolution.insert(minAt - evaluation.begin(), nextVertex);
}

Value BestTourInserter::tourObjective(const Problem& problem, Solution& partialSolution, Vertex nextVertex, std::size_t pos)
{
    const auto vertices = partialSolution.vertices();
    const auto n = vertices.size();

    const Vertex prev = vertices[(pos + n - 1) % n];
    const Vertex next = vertices[pos];

    return std::abs(partialSolution.value()
        + problem.value(prev, nextVertex) + problem.value(nextVertex, next)
        - problem.value(prev, next));
}
