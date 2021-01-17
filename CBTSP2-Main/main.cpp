#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <cassert>

import cbtsp;
import config;
import setup;
import statistics;
import util;

// Run a single search based on the given configuration.
void runFromConfiguration(const Configuration& configuration)
{
    if (configuration.inputFiles.empty()) {
        std::cout << "No input file.\n";
        return;
    }

    // random number setup
    const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto random = std::make_shared<Random>(static_cast<Random::result_type>(seed));

    const auto searchBuilder = SearchBuilder(configuration.algorithm,
        configuration.stepFunction,
        configuration.iterations, configuration.popsize,
        configuration.evaporation, configuration.elitism,
        configuration.minPheromone, configuration.maxPheromone,
        configuration.pheromoneAttraction, configuration.objectiveAttraction,
        configuration.intensification, configuration.reinforceStrategy,
        random);

    const auto search = searchBuilder.buildSearch();

    for (const auto inputFile : configuration.inputFiles) {
        std::cout << "Loading problem: " << inputFile.filename() << " - ";
        const auto problem = readProblemFile(inputFile);
        std::cout << "loaded.\n";

        const auto name = inputFile.stem().string();
        std::cout << format("Running {} searches on " + name + " - ", configuration.runs);
        const auto statistics = Statistics::measure(name, *search, problem, configuration.runs);
        std::cout << "done.\n";

        auto solutionFile = inputFile;
        solutionFile.replace_filename(name + ".solution");
        std::cout << "Recording results for " << name << " - ";
        writeResults(statistics, solutionFile, configuration.statsOutfile);
        std::cout << "written.\n";
    }

    std::cout << "All done.\n";
}

// Run the MCO benchmark, a series of MCO searches with two different parameter sets.
void runBenchMco(Configuration& configuration)
{
    configuration.algorithm = Configuration::Algorithm::MCO;
    configuration.stepFunction = Configuration::StepFunction::FIRST_IMPROVEMENT;
    configuration.iterations = 100;
    configuration.popsize = 20;
    configuration.evaporation = .1f;
    configuration.minPheromone = 0.f;
    configuration.pheromoneAttraction = 1.f;
    configuration.runs = 100;

    configuration.elitism = 0.f;
    configuration.maxPheromone = std::numeric_limits<float>::max();
    configuration.objectiveAttraction = 0.f;
    configuration.intensification = 0.f;
    configuration.reinforceStrategy = ReinforceStrategy::DARWIN;

    std::cout << "Run Raw Benchmark:\n";

    runFromConfiguration(configuration);

    configuration.elitism = 1.f;
    configuration.maxPheromone = 1.f;
    configuration.objectiveAttraction = 1.f;
    configuration.intensification = 0.5f;
    configuration.reinforceStrategy = ReinforceStrategy::LAMARCK;

    std::cout << "Run Refined Benchmark:\n";

    runFromConfiguration(configuration);
}

// Run the MCO iterations suite, an experiment to compare different popsize values.
void runPopsizeMco(Configuration& configuration)
{
    configuration.algorithm = Configuration::Algorithm::MCO;
    configuration.stepFunction = Configuration::StepFunction::FIRST_IMPROVEMENT;
    configuration.iterations = 100;
    configuration.evaporation = .1f;
    configuration.minPheromone = 0.f;
    configuration.pheromoneAttraction = 1.f;
    configuration.runs = 100;

    configuration.elitism = 0.f;
    configuration.maxPheromone = std::numeric_limits<float>::max();
    configuration.objectiveAttraction = 0.f;
    configuration.intensification = 0.f;
    configuration.reinforceStrategy = ReinforceStrategy::DARWIN;

    std::cout << "Run popsize experiment on Raw Benchmark:\n";

    for (int i = 10; i <= 100; i += 10) {
        std::cout << "Popsize " << i << ":\n";
        configuration.popsize = i;
        runFromConfiguration(configuration);
    }

    configuration.elitism = 1.f;
    configuration.maxPheromone = 1.f;
    configuration.objectiveAttraction = 1.f;
    configuration.intensification = 0.5f;
    configuration.reinforceStrategy = ReinforceStrategy::LAMARCK;

    std::cout << "Run popsize experiment on Refined Benchmark:\n";

    for (int i = 10; i <= 100; i += 10) {
        std::cout << "Popsize " << i << ":\n";
        configuration.popsize = i;
        runFromConfiguration(configuration);
    }
}

// run() is like the main function, but may throw exceptions.
void run(int argc, const char* argv[])
{
    Configuration configuration;
    configuration.readArgv(argc, argv);

    switch (configuration.suite) {
    case Configuration::Suite::SINGLE:
        runFromConfiguration(configuration);
        break;

    case Configuration::Suite::BENCH_MCO:
        runBenchMco(configuration);
        break;

    case Configuration::Suite::POPSIZE_MCO:
        runPopsizeMco(configuration);
        break;

    default:
        assert(0);

    }
}

int main(int argc, const char* argv[])
{
    try {
        run(argc, argv);
    }
    catch (const std::exception& e) {
        std::cerr << "Aborted due to error: " << e.what() << "\n";
    }
}
