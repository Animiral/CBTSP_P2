module;

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
import mco;
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
        + std::to_string(statistics.bestSolution()->objective()) + ";"
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
    int iterations, int popsize, float evaporation, float elitism,
    Pheromone minPheromone, Pheromone maxPheromone,
    float pheromoneAttraction, float objectiveAttraction,
    float intensification, ReinforceStrategy reinforceStrategy,
    const std::shared_ptr<Random>& random) noexcept
    : algorithm_(algorithm), stepFunction_(stepFunction),
    iterations_(iterations), popsize_(popsize), evaporation_(evaporation), elitism_(elitism),
    minPheromone_(minPheromone), maxPheromone_(maxPheromone),
    pheromoneAttraction_(pheromoneAttraction), objectiveAttraction_(objectiveAttraction),
    intensification_(intensification), reinforceStrategy_(reinforceStrategy),
    random_(random)
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
            buildImprovement(), iterations_);

    case Configuration::Algorithm::VND:
        return std::make_unique<Vnd>(buildRandomConstruction(), buildVndSteps());

    case Configuration::Algorithm::MCO:
        return std::make_unique<Mco>(iterations_, popsize_, evaporation_, elitism_,
            minPheromone_, maxPheromone_, pheromoneAttraction_, objectiveAttraction_,
            intensification_, reinforceStrategy_,
            random_, buildImprovement());

    default:
        assert(0);
        return {};

    }
}

std::unique_ptr<DeterministicConstruction> SearchBuilder::buildDeterministicConstruction() const
{
    auto selector = FarthestCitySelector();
    auto inserter = BestTourInserter();
    return std::make_unique<DeterministicConstruction>(selector, inserter);
}

std::unique_ptr<RandomConstruction> SearchBuilder::buildRandomConstruction() const
{
    auto selector = RandomSelector(random_);
    auto inserter = BestTourInserter();
    return std::make_unique<RandomConstruction>(selector, inserter);
}

std::unique_ptr<Neighborhood> SearchBuilder::buildFullNeighborhood() const
{
    return std::make_unique<TwoExchangeNeighborhood>();
}

std::unique_ptr<Step> SearchBuilder::buildStep(std::unique_ptr<Neighborhood> neighborhood) const
{
    switch (stepFunction_) {

    case Configuration::StepFunction::RANDOM:
        return std::make_unique<StepRandom>(move(neighborhood), random_);

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
    steps.push_back(buildStep(std::make_unique<TwoExchangeNeighborhood>(2, 2)));
    steps.push_back(buildStep(std::make_unique<NarrowNeighborhood>()));
    steps.push_back(buildStep(std::make_unique<WideNeighborhood>()));
    return steps;
}

std::unique_ptr<LocalSearch> SearchBuilder::buildImprovement() const
{
    return std::make_unique<LocalSearch>(buildStep(buildFullNeighborhood()));
}
