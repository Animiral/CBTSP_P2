module;

#include <cstddef>
#include <numeric>
#include <ranges>
#include <random>
#include <vector>
#include <cassert>

module mco;

import cbtsp;

McoState::McoState(const Problem& problem) : vertices_(problem.vertices())
{
    std::size_t edgeTableSize = vertices_ * vertices_;
    pheromone_.resize(edgeTableSize, 0.f);
    delta_.resize(edgeTableSize, 0.f);
}

const Pheromone& McoState::pheromone(Vertex a, Vertex b) const noexcept
{
    return pheromone_[a * vertices_ + b];
}

void McoState::reinforce(Vertex a, Vertex b, Pheromone delta) noexcept
{
    delta_[a * vertices_ + b] += delta;
    delta_[b * vertices_ + a] += delta;
}

void McoState::evaporate(float evaporation) noexcept
{
    for (Pheromone& p : pheromone_)
    {
        p *= (1.f - evaporation);
    }
}

void McoState::update() noexcept
{
    for (std::size_t i = 0; i < pheromone_.size(); i++) {
        pheromone_[i] += delta_[i];
    }

    std::ranges::fill(delta_, 0.f);
}

Mouse::Mouse(const Problem& problem, McoState& state,
    float pheromoneAttraction, float objectiveAttraction,
    const std::shared_ptr<Random>& random) noexcept
    : problem_(&problem), state_(&state),
    pheromoneAttraction_(pheromoneAttraction), objectiveAttraction_(objectiveAttraction),
    random_(random)
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
        solution.twoOpt(0, start + 1);

    for (std::size_t i = 1; i < n; i++) {
        // find next vertex based on neighbors and pheromones, >i
        std::size_t next = decideNext(solution, i);

        if (next > i) {
            solution.twoOpt(i, next + 1);
        }
    }

    // we got our solution, now do reinforcement
    const Pheromone delta = 1.f / static_cast<Pheromone>(solution.objective());
    const auto& vs = solution.vertices();
    Vertex prev = vs.back();
    for (auto v : vs) {
        state_->reinforce(prev, v, delta);
        prev = v;
    }

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

    auto distribution = std::discrete_distribution<std::size_t>{ incentive.begin(), incentive.end() };
    return distribution(*random_) + position;
}

Mco::Mco(int ticks, int mice, float evaporation,
    float pheromoneAttraction, float objectiveAttraction,
    const std::shared_ptr<Random>& random) noexcept
    : ticks_(ticks), mice_(mice), evaporation_(evaporation),
    pheromoneAttraction_(pheromoneAttraction), objectiveAttraction_(objectiveAttraction),
    random_(random)
{
    assert(ticks > 0);
    assert(mice > 0);
    assert(evaporation >= 0.f);
    assert(evaporation <= 1.f);
    assert(random);
}

Solution Mco::search(const Problem& problem)
{
    auto state = McoState{ problem };
    auto mouse = Mouse{ problem, state, pheromoneAttraction_, objectiveAttraction_, random_ };
    auto best = Solution{ problem, {}, std::numeric_limits<Value>::max() };
    auto candidates = std::vector<Solution>(mice_, best);
    auto countdown = ticks_;

    while (countdown-- > 0) {
        for (std::size_t i = 0; i < mice_; i++) {
            candidates[i] = mouse.construct();

            if (candidates[i] < best) {
                best = candidates[i];
                countdown = ticks_;
            }
        }

        state.update();
        state.evaporate(evaporation_);
    }

    return best;
}
