module;

#include <ranges>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>
#include <chrono>

module statistics;

// ---- helpers ----

bool isFeasible(const Solution& s)
{
    return s.isFeasible();
}

bool isInfeasible(const Solution& s)
{
    return !s.isFeasible();
}

// ---- Statistics member functions ----

Statistics::Statistics(const std::string& name)
    : name_(name)
{
}

void Statistics::record(const Solution& solution, Runtime runtime)
{
    solutions_.emplace_back(solution);
    runtimes_.push_back(runtime);
}

const std::string& Statistics::name() const noexcept
{
    return name_;
}

int Statistics::samples() const noexcept
{
    return static_cast<int>(solutions_.size());
}

int Statistics::feasibles() const noexcept
{
    return static_cast<int>(std::ranges::count_if(solutions_, isFeasible));
}

const Solution* Statistics::bestSolution() const noexcept
{
    return &*std::ranges::min_element(solutions_, std::less<Solution>{});
}

float Statistics::meanObjective() const noexcept
{
    const auto feasibleSolutions = std::ranges::filter_view(solutions_, isFeasible);

    const auto n = feasibles();
    const auto getObjective = [n](const Solution& s) { return static_cast<float>(s.objective()) / n; };
    auto objectives = std::ranges::transform_view(feasibleSolutions, getObjective);

    return std::accumulate(objectives.begin(), objectives.end(), 0.f);
}

float Statistics::stdevObjective() const noexcept
{
    const auto feasibleSolutions = std::ranges::filter_view(solutions_, isFeasible);

    // good approximation for unbiased sample standard deviation
    // see https://en.wikipedia.org/wiki/Standard_deviation#Unbiased_sample_standard_deviation
    const auto n = feasibles();
    const auto mean = meanObjective();
    const auto getSqdev = [n, mean](const Solution& s)
    {
        const auto dev = static_cast<float>(s.objective()) - mean;
        return dev * dev / (static_cast<float>(n) - 1.5f);
    };
    auto sqdevs = std::ranges::transform_view(feasibleSolutions, getSqdev);
    return std::sqrt(std::accumulate(sqdevs.begin(), sqdevs.end(), 0.f));
}

float Statistics::meanInfEdges() const noexcept
{
    const auto infeasibles = std::ranges::filter_view(solutions_, isInfeasible);

    const auto n = solutions_.size() - feasibles();
    const auto getInfEdges = [n](const Solution& s) { return static_cast<float>(s.countInfeasibleEdges()) / n; };
    auto infEdges = std::ranges::transform_view(infeasibles, getInfEdges);

    return std::accumulate(infEdges.begin(), infEdges.end(), 0.f);
}

float Statistics::stdevInfEdges() const noexcept
{
    const auto infeasibles = std::ranges::filter_view(solutions_, isInfeasible);

    // good approximation for unbiased sample standard deviation
    // see https://en.wikipedia.org/wiki/Standard_deviation#Unbiased_sample_standard_deviation
    const auto n = solutions_.size() - feasibles();
    const auto mean = meanInfEdges();
    const auto getSqdev = [n, mean](const Solution& s)
    {
        const auto dev = static_cast<float>(s.countInfeasibleEdges()) - mean;
        return dev * dev / (static_cast<float>(n) - 1.5f);
    };
    auto sqdevs = std::ranges::transform_view(infeasibles, getSqdev);
    return std::sqrt(std::accumulate(sqdevs.begin(), sqdevs.end(), 0.f));
}

Runtime Statistics::medRuntime() const noexcept
{
    const auto n = runtimes_.size();
    assert(n > 0);

    if (1 == n % 2) {
        return runtimes_[n / 2];
    }
    else {
        return (runtimes_[n / 2] + runtimes_[n / 2 + 1]) / 2;
    }
}

Statistics Statistics::measure(const std::string& name, Search& search, const Problem& problem, int samples)
{
    assert(samples > 0);

    Statistics statistics{ name };

    for (int i = 0; i < samples; i++) {
        const auto start = Clock::now();
        const auto& solution = search.search(problem);
        auto stop = Clock::now();
        statistics.record(solution, stop - start);
    }

    return statistics;
}
