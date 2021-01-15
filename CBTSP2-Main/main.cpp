#include <iostream>
#include <string>
#include <random>
#include <chrono>

import cbtsp;
import config;
import setup;
import statistics;
import util;

// run() is like the main function, but may throw exceptions.
int run(int argc, const char* argv[])
{
    Configuration configuration;
    configuration.readArgv(argc, argv);

    if (configuration.inputFiles().empty()) {
        std::cout << "No input file.\n";
        return {};
    }

    // random number setup
    const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto random = std::make_shared<Random>(static_cast<Random::result_type>(seed));

    for (const auto inputFile : configuration.inputFiles()) {
        std::cout << "Loading problem: " << inputFile.filename() << " - ";
        const auto problem = readProblemFile(inputFile);
        std::cout << "loaded.\n";

        const auto searchBuilder = SearchBuilder(configuration.algorithm(),
            configuration.stepFunction(),
            problem,
            configuration.iterations(),
            configuration.popsize(),
            random);

        const auto search = searchBuilder.buildSearch();

        const auto name = inputFile.stem().string();
        std::cout << format("Running {} searches on " + name + " - ", configuration.runs());
        const auto statistics = Statistics::measure(name, *search, problem, configuration.runs());
        std::cout << "done.\n";

        auto solutionFile = inputFile;
        solutionFile.replace_filename(name + ".solution");
        std::cout << "Recording results for " << name << " - ";
        writeResults(statistics, solutionFile, configuration.statsOutfile());
        std::cout << "written.\n";
    }

    std::cout << "All done.\n";
    return 0;
}

int main(int argc, const char* argv[])
{
    try {
        return run(argc, argv);
    }
    catch (const std::exception& e) {
        std::cerr << "Aborted due to error: " << e.what() << "\n";
    }
}
