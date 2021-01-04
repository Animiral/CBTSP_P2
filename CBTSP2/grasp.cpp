module;

#include <utility>

module grasp;

AfterIterations::AfterIterations(int iterations) noexcept
    : iterations_(iterations)
{
}

bool AfterIterations::doneAfter(const Solution& ) noexcept
{
    return iterations_-- <= 0;
}

Grasp::Grasp(RandomConstruction&& construction, LocalSearch&& localSearch, AfterIterations doTerminate) noexcept
    : construction_(construction), localSearch_(std::move(localSearch)), doTerminate_(doTerminate)
{
}

Solution Grasp::search(const Problem& problem)
{
    std::vector<Vertex> vs;
    return Solution{ problem, move(vs) }; // TODO: translate Python
//solution = self.search(self.construction(instance))
//solution_objective = self.objective(solution)
//while not self.done(self, solution) :
//    candidate = self.search(self.construction(instance))
//    candidate_objective = self.objective(candidate)
//    if candidate_objective < solution_objective :
//        solution = candidate

//return solution
}