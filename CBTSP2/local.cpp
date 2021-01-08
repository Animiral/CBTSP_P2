module;

#include <limits>
#include <memory>
#include <utility>
#include <cassert>

module local;

Neighborhood::Neighborhood(std::size_t vertices) noexcept
    : vertices_(vertices)
{
}

Solution Neighborhood::applyCopy(const Solution& base) const
{
    Solution neighbor{ base };
    apply(neighbor);
    return neighbor;
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(std::size_t vertices,
    std::size_t minl, std::size_t maxl) noexcept
    : Neighborhood(vertices), minl_(minl), maxl_(maxl), cut1_(0), cut2_(minl)
{
    assert(minl >= 2);
    assert(maxl >= minl);
    assert(maxl <= vertices);
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(std::size_t vertices, std::size_t minl) noexcept
    : TwoExchangeNeighborhood(vertices, minl, vertices)
{
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> TwoExchangeNeighborhood::clone() const
{
    return std::make_unique<TwoExchangeNeighborhood>(*this);
}

TwoExchangeNeighborhood& TwoExchangeNeighborhood::operator++()
{
    std::size_t shorterSubtour; // length of shorter subtour, may be restricted

    do {
        cut2_++;

        if (cut2_ >= vertices_) {
            cut1_++;
            cut2_ = cut1_ + minl_;

            if (cut1_ >= vertices_ - minl_)
                break;
        }

        shorterSubtour = std::min(cut2_ - cut1_, cut1_ + vertices_ - cut2_);
    } while (shorterSubtour < minl_ || shorterSubtour > maxl_);

    return *this;
}

Value TwoExchangeNeighborhood::objective(const Solution& base) const noexcept
{
    return std::abs(base.twoOptValue(cut1_, cut2_));
}

void TwoExchangeNeighborhood::apply(Solution& solution) const
{
    solution.twoOpt(cut1_, cut2_);
}

bool TwoExchangeNeighborhood::operator!=(std::default_sentinel_t) const noexcept
{
    return cut1_ < vertices_ - minl_;
}

Step::Step(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : neighborhood_(move(neighborhood))
{
}

FirstImprovement::FirstImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : Step(move(neighborhood))
{
}

void FirstImprovement::step(Solution& base)
{
    auto baseObjective = base.objective();

    for (auto ns = neighborhood_->clone(); *ns != std::default_sentinel; ++*ns) {
        if (ns->objective(base) < baseObjective) {
            ns->apply(base);
            return;
        }
    }
}

BestImprovement::BestImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : Step(move(neighborhood))
{
}

void BestImprovement::step(Solution& base)
{
    auto bestObjective = base.objective();
    std::unique_ptr<Neighborhood> bestNeighbor = nullptr;

    for (auto ns = neighborhood_->clone(); *ns != std::default_sentinel; ++*ns) {
        const Value newObjective = ns->objective(base);
        if (newObjective < bestObjective) {
            bestObjective = newObjective;
            bestNeighbor = ns->clone();
        }
    }

    if(bestNeighbor)
        bestNeighbor->apply(base);
}

WhenStagnant::WhenStagnant() noexcept
    : best_(std::numeric_limits<Value>::max())
{
}

bool WhenStagnant::doneAfter(const Solution& solution) noexcept
{
    auto objective = solution.objective();
    if (objective >= best_) {
        return true;
    }
    else {
        best_ = objective;
        return false;
    }
}

LocalSearch::LocalSearch(std::unique_ptr<Step> step, WhenStagnant doTerminate) noexcept
    : step_(move(step)), doTerminate_(doTerminate)
{
}

Solution LocalSearch::search(Solution solution)
{
    do {
        step_->step(solution);
    } while (!doTerminate_.doneAfter(solution));

    return solution;
}

StandaloneLocalSearch::StandaloneLocalSearch(
    std::unique_ptr<Construction> construction,
    std::unique_ptr<Step> step,
    WhenStagnant doTerminate) noexcept
    : construction_(move(construction)), local_(move(step), doTerminate)
{
}

Solution StandaloneLocalSearch::search(const Problem& problem)
{
    return local_.search(construction_->construct(problem));
}
