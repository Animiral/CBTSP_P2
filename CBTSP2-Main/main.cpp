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
        configuration.pheromoneAttraction, configuration.objectiveAttraction, configuration.reinforceStrategy,
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

// Run the MCO benchmark, a series of MCO searches with different parameters.
void runBenchMco(Configuration& configuration)
{
    std::cout << "Run MCO Benchmark:\n";
    configuration.algorithm = Configuration::Algorithm::MCO;
    runFromConfiguration(configuration);
}

// Run the MCO iterations suite, an experiment to compare different popsize values.
void runPopsizeMco(Configuration& configuration)
{
    std::cout << "Run MCO Popsize Experiment:\n";
    configuration.algorithm = Configuration::Algorithm::MCO;
    for (int i = 1; i <= 2048; i *= 2) {
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
