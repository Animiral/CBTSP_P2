module;

#include <memory>
#include <utility>
#include <cassert>

module grasp;

Grasp::Grasp(std::unique_ptr<Construction> construction, LocalSearch&& localSearch, int iterations) noexcept
    : construction_(move(construction)), localSearch_(std::move(localSearch)), iterations_(iterations)
{
    assert(iterations > 0);
}

Solution Grasp::search(const Problem& problem)
{
    Solution solution = localSearch_.search(construction_->construct(problem));

    for (int i = 1; i < iterations_; i++) {
        Solution candidate = localSearch_.search(construction_->construct(problem));
        if (candidate < solution) {
            solution = std::move(candidate);
        }
    }

    return solution;
}