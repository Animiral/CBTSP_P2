module;

#include <memory>
#include <utility>

module vnd;

Vnd::Vnd(std::unique_ptr<Construction> construction, std::vector<std::unique_ptr<Step>> steps) noexcept
    : construction_(move(construction)), steps_(move(steps))
{
}

Solution Vnd::search(const Problem& problem)
{
    Solution best = construction_->construct(problem);
    std::size_t level = 0;

    while (level < steps_.size()) {
        Solution candidate = best;
        steps_[level]->step(candidate);

        if (candidate.objective() < best.objective()) {
            best = std::move(candidate);
            level = 0;
        }
        else {
            level++; // widen search
        }
    }

    return best;
}
