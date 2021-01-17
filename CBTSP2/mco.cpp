module;

#include <cstddef>
#include <numeric>
#include <ranges>
#include <random>
#include <vector>
#include <cassert>

module mco;

import cbtsp;

McoState::McoState(const Problem& problem, Pheromone init, Pheromone min, Pheromone max)
    : vertices_(problem.vertices()), min_(min), max_(max),
    pheromone_(vertices_, init), delta_(vertices_, 0.f)
{
}

Pheromone McoState::pheromone(Vertex a, Vertex b) const noexcept
{
    return pheromone_.at(a, b);
}

void McoState::reinforce(const Solution& solution, float scale) noexcept
{
    assert(!solution.isPartial());

    const Pheromone delta = scale / static_cast<Pheromone>(solution.objective());
    const auto& vs = solution.vertices();
    Vertex prev = vs.back();
    for (auto v : vs) {
        delta_.at(prev, v) += delta;
        prev = v;
    }
}

void McoState::evaporate(float evaporation) noexcept
{
    for (Pheromone& p : pheromone_.all())
    {
        p = (1.f - evaporation) * p + evaporation * min_;
    }
}

void McoState::update() noexcept
{
    auto& pheromone = pheromone_.all();
    auto& delta = delta_.all();

    for (std::size_t i = 0; i < pheromone.size(); i++) {
        pheromone[i] = std::clamp(pheromone[i] + delta[i], min_, max_);
    }

    std::ranges::fill(delta, 0.f);
}

Mouse::Mouse(const Problem& problem, McoState& state,
    float pheromoneAttraction, float objectiveAttraction,
    float intensification, const std::shared_ptr<Random>& random) noexcept
    : problem_(&problem), state_(&state),
    pheromoneAttraction_(pheromoneAttraction), objectiveAttraction_(objectiveAttraction),
    intensification_(intensification), random_(random)
{
    assert(random);
}

Solution Mouse::construct()
{
    const std::size_t n = problem_->vertices();
    std::vector<Vertex> tour(n, 0);
    std::iota(tour.begin(), tour.end(), 0);
    std::ranges::shuffle(tour, *random_);

    Solution solution(*problem_, move(tour));

    // starting location is random
    std::size_t start = std::uniform_int_distribution{ 0ull, n - 1 }(*random_);
    if (start > 0)
        solution.twoOpt(0, (start + 1) % n);

    for (std::size_t i = 1; i < n; i++) {
        // find next vertex based on neighbors and pheromones
        const auto next = decideNext(solution, i);
        solution.twoOpt(i, (next + 1) % n);
    }

    // Note: reinforcement to be handled by the caller
    return solution;
}

std::size_t Mouse::decideNext(const Solution& solution, std::size_t position)
{
    assert(position > 0); // first vertex must be decided at random

    const std::size_t n = problem_->vertices();
    std::vector<Pheromone> incentive(n - position, 0);

    const auto from = solution.vertices()[position - 1];

    for (std::size_t i = position; i < n; i++) {
        const auto to = solution.vertices()[i];
        const auto pheromone = state_->pheromone(from, to);
        const auto objective = 1.f / std::abs(solution.twoOptValue(0, i));
        incentive[i - position] = std::pow(pheromone, pheromoneAttraction_)
            + std::pow(objective, objectiveAttraction_);
    }

    // diversification or intensification?
    if (std::generate_canonical<float, std::numeric_limits<float>::digits>(*random_) < intensification_) {
        // choose max incentivized
        return std::ranges::max_element(incentive) - incentive.begin() + position;
    }
    else {
        // choose according to incentives as probabilities
        auto distribution = std::discrete_distribution<std::size_t>{ incentive.begin(), incentive.end() };
        return distribution(*random_) + position;
    }
}

Mco::Mco(int ticks, int mice, float evaporation, float elitism,
    float minPheromone, float maxPheromone,
    float pheromoneAttraction, float objectiveAttraction,
    float intensification, ReinforceStrategy reinforceStrategy,
    const std::shared_ptr<Random>& random, std::unique_ptr<LocalSearch> improvement) noexcept
    : ticks_(ticks), mice_(mice), evaporation_(evaporation), elitism_(elitism),
    minPheromone_(minPheromone), maxPheromone_(maxPheromone),
    pheromoneAttraction_(pheromoneAttraction), objectiveAttraction_(objectiveAttraction),
    intensification_(intensification), reinforceStrategy_(reinforceStrategy),
    random_(move(random)), improvement_(move(improvement))
{
    assert(ticks > 0);
    assert(mice > 0);
    assert(evaporation >= 0.f);
    assert(evaporation <= 1.f);
    assert(elitism >= 0.f);
    assert(minPheromone < maxPheromone);
    assert(intensification >= 0.f);
    assert(intensification <= 1.f);
    assert(random_);
    assert(improvement_);
}

Solution Mco::search(const Problem& problem)
{
    auto state = McoState{ problem, maxPheromone_, minPheromone_, maxPheromone_ };
    auto mouse = Mouse{ problem, state, pheromoneAttraction_, objectiveAttraction_,
        intensification_, random_ };
    auto best = Solution{ problem, {}, std::numeric_limits<Value>::max() };
    auto candidates = std::vector<Solution>(mice_, best);
    auto countdown = ticks_;

    while (countdown-- > 0) {
        for (std::size_t i = 0; i < mice_; i++) {
            const auto constructed = mouse.construct();
            const auto improved = improvement_->search(std::move(constructed));
            state.reinforce(ReinforceStrategy::DARWIN == reinforceStrategy_ ? constructed : improved);
            candidates[i] = std::move(improved);

            if (candidates[i] < best) {
                best = candidates[i];
                countdown = ticks_;
            }
        }

        state.reinforce(best, elitism_); // best known solution gets extra pheromones
        state.update();
        state.evaporate(evaporation_);
    }

    return best;
}
