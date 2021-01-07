module;

#include <limits>
#include <memory>
#include <utility>

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

TwoExchangeNeighborhood::TwoExchangeNeighborhood(std::size_t vertices) noexcept
    : Neighborhood(vertices), cut1_(0), cut2_(2)
{
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> TwoExchangeNeighborhood::clone() const
{
    return std::make_unique<TwoExchangeNeighborhood>(*this);
}

TwoExchangeNeighborhood& TwoExchangeNeighborhood::operator++()
{
    cut2_++;

    if (cut2_ >= vertices_ - int(0 == cut1_)) {
        cut1_++;
        cut2_ = cut1_ + 2;
    }

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
    return cut1_ < vertices_ - 2;
}

NarrowTwoExchangeNeighborhood::NarrowTwoExchangeNeighborhood(std::size_t vertices) noexcept
    : Neighborhood(vertices), maxl_(std::max(vertices / 4, 3ull)), cut1_(0), cut2_(3)
{
}

NarrowTwoExchangeNeighborhood::NarrowTwoExchangeNeighborhood(const NarrowTwoExchangeNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> NarrowTwoExchangeNeighborhood::clone() const
{
    return std::make_unique<NarrowTwoExchangeNeighborhood>(*this);
}

NarrowTwoExchangeNeighborhood& NarrowTwoExchangeNeighborhood::operator++()
{
    do {
        cut2_++;

        if (cut2_ >= vertices_) {
            cut1_++;
            cut2_ = cut1_ + 3;

            if (cut1_ >= vertices_)
                break;
        }
    } while ((cut2_ - cut1_ > maxl_ && cut1_ + vertices_ - cut2_ > maxl_) || cut1_ + vertices_ - cut2_ < 3);

    return *this;
}

Value NarrowTwoExchangeNeighborhood::objective(const Solution& base) const noexcept
{
    return std::abs(base.twoOptValue(cut1_, cut2_));
}

void NarrowTwoExchangeNeighborhood::apply(Solution& solution) const
{
    solution.twoOpt(cut1_, cut2_);
}

bool NarrowTwoExchangeNeighborhood::operator!=(std::default_sentinel_t) const noexcept
{
    return cut1_ < vertices_;
}

WideTwoExchangeNeighborhood::WideTwoExchangeNeighborhood(std::size_t vertices) noexcept
    : Neighborhood(vertices), minl_(std::max(vertices / 4, 3ull) + 1), cut1_(0), cut2_(minl_)
{
}

WideTwoExchangeNeighborhood::WideTwoExchangeNeighborhood(const WideTwoExchangeNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> WideTwoExchangeNeighborhood::clone() const
{
    return std::make_unique<WideTwoExchangeNeighborhood>(*this);
}

WideTwoExchangeNeighborhood& WideTwoExchangeNeighborhood::operator++()
{
    do {
        cut2_++;

        if (cut2_ >= vertices_) {
            cut1_++;
            cut2_ = cut1_ + minl_;

            if (cut1_ >= vertices_)
                break;
        }
    } while (cut2_ - cut1_ < minl_ || cut1_ + vertices_ - cut2_ < minl_);

    return *this;
}

Value WideTwoExchangeNeighborhood::objective(const Solution& base) const noexcept
{
    return std::abs(base.twoOptValue(cut1_, cut2_));
}

void WideTwoExchangeNeighborhood::apply(Solution& solution) const
{
    solution.twoOpt(cut1_, cut2_);
}

bool WideTwoExchangeNeighborhood::operator!=(std::default_sentinel_t) const noexcept
{
    return cut1_ < vertices_;
}

VertexShiftNeighborhood::VertexShiftNeighborhood(std::size_t vertices) noexcept
: Neighborhood(vertices), cut_(0)
{
}

VertexShiftNeighborhood::VertexShiftNeighborhood(const VertexShiftNeighborhood& rhs) = default;

std::unique_ptr<Neighborhood> VertexShiftNeighborhood::clone() const
{
    return std::make_unique<VertexShiftNeighborhood>(*this);
}

VertexShiftNeighborhood& VertexShiftNeighborhood::operator++()
{
    cut_++;
    return *this;
}

Value VertexShiftNeighborhood::objective(const Solution& base) const noexcept
{
    return std::abs(base.twoOptValue(cut_, (cut_ + 2) % vertices_));
}

void VertexShiftNeighborhood::apply(Solution& solution) const
{
    solution.twoOpt(cut_, (cut_ + 2) % vertices_);
}

bool VertexShiftNeighborhood::operator!=(std::default_sentinel_t) const noexcept
{
    return cut_ < vertices_;
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
    DeterministicConstruction&& construction,
    std::unique_ptr<Step> step,
    WhenStagnant doTerminate) noexcept
    : construction_(std::move(construction)), local_(move(step), doTerminate)
{
}

Solution StandaloneLocalSearch::search(const Problem& problem)
{
    return local_.search(construction_.construct(problem));
}
