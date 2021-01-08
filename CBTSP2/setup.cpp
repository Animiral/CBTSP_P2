module;

#include <random>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <ios>
#include <cassert>

module setup;

import cbtsp;
import config;
import construction;
import local;
import grasp;
import vnd;
import aco;
import util;

Problem readProblemFile(std::filesystem::path filePath)
{
    auto stream = std::ifstream{ filePath };
    std::string contents;

    stream.seekg(0, std::ios::end);
    contents.reserve(narrow_cast<std::size_t>(static_cast<std::streamoff>(stream.tellg())));
    stream.seekg(0, std::ios::beg);

    contents.assign((std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    stream.close();

    if (!stream)
        throw std::runtime_error("Error reading problem from " + filePath.string());

    return Problem::fromText(contents);
}

void writeResults(const Statistics& statistics, std::filesystem::path solutionPath, std::filesystem::path statsOutPath)
{
    auto stream = std::ofstream{ solutionPath, std::ios_base::out }; // overwrite
    stream << statistics.bestSolution()->representation() << "\n";
    stream.close();

    if (statsOutPath.empty())
        return; // do not write stats

    using fracSecs = std::chrono::duration<float>;

    auto csv = statistics.name() + ";"
        + std::to_string(statistics.samples()) + ";"
        + std::to_string(statistics.feasibles()) + ";"
        + std::to_string(statistics.meanObjective()) + ";"
        + std::to_string(statistics.stdevObjective()) + ";"
        + std::to_string(statistics.meanInfEdges()) + ";"
        + std::to_string(statistics.stdevInfEdges()) + ";"
        + std::to_string(std::chrono::duration_cast<fracSecs>(statistics.medRuntime()).count());
    stream.open(statsOutPath, std::ios_base::app); // append CSV
    stream << csv << "\n";
    stream.close();

    if (!stream)
        throw std::runtime_error("Error writing stats to " + statsOutPath.string());
}

SearchBuilder::SearchBuilder(Configuration::Algorithm algorithm,
    Configuration::StepFunction stepFunction,
    const Problem& problem,
    int iterations) noexcept
    : algorithm_(algorithm), stepFunction_(stepFunction), problem_(&problem), iterations_(iterations)
{
}

std::unique_ptr<Search> SearchBuilder::buildSearch() const
{
    switch (algorithm_) {

    case Configuration::Algorithm::DET_CONSTRUCTION:
        return std::make_unique<ConstructionSearch>(buildDeterministicConstruction());

    case Configuration::Algorithm::RAND_CONSTRUCTION:
        return std::make_unique<ConstructionSearch>(buildRandomConstruction());

    case Configuration::Algorithm::LOCAL_SEARCH:
        return std::make_unique<StandaloneLocalSearch>(buildDeterministicConstruction(),
            buildStep(buildFullNeighborhood()));

    case Configuration::Algorithm::GRASP:
        return std::make_unique<Grasp>(buildRandomConstruction(),
            LocalSearch(buildStep(buildFullNeighborhood())), iterations_);

    case Configuration::Algorithm::VND:
        return std::make_unique<Vnd>(buildRandomConstruction(), buildVndSteps());

    case Configuration::Algorithm::ACO:
        return std::make_unique<Aco>(); // not implemented yet

    default:
        assert(0);
        return {};

    }
}

std::default_random_engine SearchBuilder::buildRandom() const
{
    const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    return std::default_random_engine{ static_cast<std::default_random_engine::result_type>(seed) };
}

std::unique_ptr<DeterministicConstruction> SearchBuilder::buildDeterministicConstruction() const
{
    auto selector = FarthestCitySelector();
    auto inserter = BestTourInserter();
    return std::make_unique<DeterministicConstruction>(selector, inserter);
}

std::unique_ptr<RandomConstruction> SearchBuilder::buildRandomConstruction() const
{
    auto selector = RandomSelector(buildRandom());
    auto inserter = BestTourInserter();
    return std::make_unique<RandomConstruction>(selector, inserter);
}

std::unique_ptr<Neighborhood> SearchBuilder::buildFullNeighborhood() const
{
    return std::make_unique<TwoExchangeNeighborhood>(problem_->vertices());
}

std::unique_ptr<Neighborhood> SearchBuilder::buildShiftNeighborhood() const
{
    return std::make_unique<TwoExchangeNeighborhood>(problem_->vertices(), 2, 2);
}

std::unique_ptr<Neighborhood> SearchBuilder::buildNarrowNeighborhood() const
{
    return std::make_unique<TwoExchangeNeighborhood>(problem_->vertices(), 3, std::max(problem_->vertices() / 4, 3ull));
}

std::unique_ptr<Neighborhood> SearchBuilder::buildWideNeighborhood() const
{
    return std::make_unique<TwoExchangeNeighborhood>(problem_->vertices(), std::max(problem_->vertices() / 4, 3ull) + 1);
}

std::unique_ptr<Step> SearchBuilder::buildStep(std::unique_ptr<Neighborhood> neighborhood) const
{
    switch (stepFunction_) {

    case Configuration::StepFunction::RANDOM:
        using StepRandomImpl = StepRandom<std::default_random_engine>;
        return std::make_unique<StepRandomImpl>(move(neighborhood), buildRandom());

    case Configuration::StepFunction::FIRST_IMPROVEMENT:
        return std::make_unique<FirstImprovement>(move(neighborhood));

    case Configuration::StepFunction::BEST_IMPROVEMENT:
        return std::make_unique<BestImprovement>(move(neighborhood));

    default:
        assert(0);
        return {};

    }
}

std::vector<std::unique_ptr<Step>> SearchBuilder::buildVndSteps() const
{
    auto steps = std::vector<std::unique_ptr<Step>>();
    steps.push_back(buildStep(buildShiftNeighborhood()));
    steps.push_back(buildStep(buildNarrowNeighborhood()));
    steps.push_back(buildStep(buildWideNeighborhood()));
    return steps;
}
