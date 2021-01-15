module;

#include <memory>
#include <utility>
#include <cassert>

module grasp;

Grasp::Grasp(std::unique_ptr<Construction> construction,
    std::unique_ptr<LocalSearch> improvement, int iterations) noexcept
    : construction_(move(construction)), improvement_(move(improvement)), iterations_(iterations)
{
    assert(iterations > 0);
}

Solution Grasp::search(const Problem& problem)
{
    Solution solution = improvement_->search(construction_->construct(problem));

    for (int i = 1; i < iterations_; i++) {
        Solution candidate = improvement_->search(construction_->construct(problem));
        if (candidate < solution) {
            solution = std::move(candidate);
        }
    }

    return solution;
}