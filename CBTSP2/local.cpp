module;

#include <limits>
#include <memory>
#include <utility>
#include <random>
#include <cassert>

module local;

void Neighborhood::reset(std::size_t vertices) noexcept
{
    vertices_ = vertices;
}

Solution Neighborhood::applyCopy(const Solution& base) const
{
    Solution neighbor{ base };
    apply(neighbor);
    return neighbor;
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(std::size_t minl, std::size_t maxl) noexcept
    : Neighborhood(), minl_(minl), maxl_(maxl), cut1_(0), cut2_(minl)
{
    assert(minl >= 2);
    assert(maxl >= minl);
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(std::size_t minl) noexcept
    : TwoExchangeNeighborhood(minl, std::numeric_limits<std::size_t>::max())
{
}

void TwoExchangeNeighborhood::reset(std::size_t vertices) noexcept
{
    Neighborhood::reset(vertices);
    cut1_ = 0;
    cut2_ = minl_;
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> TwoExchangeNeighborhood::clone() const
{
    return std::make_unique<TwoExchangeNeighborhood>(*this);
}

TwoExchangeNeighborhood& TwoExchangeNeighborhood::operator++()
{
    assert(cut1_ < vertices_ - minl_);

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

NarrowNeighborhood::NarrowNeighborhood() noexcept
    : TwoExchangeNeighborhood(3ull, 3ull)
{
}

void NarrowNeighborhood::reset(std::size_t vertices) noexcept
{
    TwoExchangeNeighborhood::reset(vertices);
    maxl_ = std::max(vertices / 4, 3ull);
}

std::unique_ptr<Neighborhood> NarrowNeighborhood::clone() const
{
    return std::make_unique<NarrowNeighborhood>(*this);
}

WideNeighborhood::WideNeighborhood() noexcept
    : TwoExchangeNeighborhood(3ull)
{
}

void WideNeighborhood::reset(std::size_t vertices) noexcept
{
    TwoExchangeNeighborhood::reset(vertices);
    minl_ = std::max(vertices / 4, 3ull) + 1;
}

std::unique_ptr<Neighborhood> WideNeighborhood::clone() const
{
    return std::make_unique<WideNeighborhood>(*this);
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

    for (neighborhood_->reset(base.length()); *neighborhood_ != std::default_sentinel; ++*neighborhood_) {
        if (neighborhood_->objective(base) < baseObjective) {
            neighborhood_->apply(base);
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

    for (neighborhood_->reset(base.length()); *neighborhood_ != std::default_sentinel; ++*neighborhood_) {
        const Value newObjective = neighborhood_->objective(base);
        if (newObjective < bestObjective) {
            bestObjective = newObjective;
            bestNeighbor = neighborhood_->clone();
        }
    }

    if(bestNeighbor)
        bestNeighbor->apply(base);
}

StepRandom::StepRandom(std::unique_ptr<Neighborhood> neighborhood,
    const std::shared_ptr<Random>& random) noexcept
    : Step(move(neighborhood)), random_(random)
{
    assert(random);
}

void StepRandom::step(Solution& base)
{
    // extremely stupid count, since neighborhood does not know its own size
    int neighbors = 0;

    for (neighborhood_->reset(base.length()); *neighborhood_ != std::default_sentinel; ++*neighborhood_)
        neighbors++;

    if (neighbors > 0) {
        const auto distribution = std::uniform_int_distribution{ 0, neighbors - 1 };
        const int choice = distribution(*random_);
        neighborhood_->reset(base.length());

        for (int i = 0; i < choice; i++)
            ++*neighborhood_;

        neighborhood_->apply(base);
    }
}

LocalSearch::LocalSearch(std::unique_ptr<Step> step) noexcept
    : step_(move(step))
{
}

Solution LocalSearch::search(Solution solution)
{
    auto best = solution.objective();

    for (;;) {
        step_->step(solution);

        const auto objective = solution.objective();

        if (objective >= best)
            break;
        else
            best = objective;
    }

    return solution;
}

StandaloneLocalSearch::StandaloneLocalSearch(
    std::unique_ptr<Construction> construction,
    std::unique_ptr<Step> step) noexcept
    : construction_(move(construction)), local_(move(step))
{
}

Solution StandaloneLocalSearch::search(const Problem& problem)
{
    return local_.search(construction_->construct(problem));
}
