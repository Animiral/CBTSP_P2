module;

#include <memory>
#include <utility>

module grasp;

AfterIterations::AfterIterations(int iterations) noexcept
    : iterations_(iterations)
{
}

bool AfterIterations::done() noexcept
{
    return iterations_-- <= 0;
}

Grasp::Grasp(std::unique_ptr<Construction> construction, LocalSearch&& localSearch, AfterIterations doTerminate) noexcept
    : construction_(move(construction)), localSearch_(std::move(localSearch)), doTerminate_(doTerminate)
{
}

Solution Grasp::search(const Problem& problem)
{
    Solution solution{ problem, {} };

    if (!doTerminate_.done()) {
        solution = localSearch_.search(construction_->construct(problem));
    }

    while (!doTerminate_.done()) {
        Solution candidate = localSearch_.search(construction_->construct(problem));
        if (candidate.objective() < solution.objective()) {
            solution = std::move(candidate);
        }
    }

    return solution;
}