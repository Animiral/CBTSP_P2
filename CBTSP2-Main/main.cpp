#include <iostream>
#include <string>

import cbtsp;
import config;
import setup;
import statistics;
import util;

int main(int argc, const char* argv[])
{
    Configuration configuration;
    configuration.readArgv(argc, argv);

    if (configuration.inputFiles().empty()) {
        std::cout << "No input file.\n";
        return {};
    }

    for (const auto inputFile : configuration.inputFiles()) {
        std::cout << "Loading problem: " << inputFile.filename() << " - ";
        const auto problem = readProblemFile(configuration.inputFiles().front());
        std::cout << "loaded.\n";

        const auto searchBuilder = SearchBuilder(configuration.algorithm(),
            configuration.stepFunction(),
            problem,
            configuration.iterations());

        const auto search = searchBuilder.buildSearch();

        const auto name = inputFile.stem().string();
        std::cout << format("Running {} searches on " + name + " - ", configuration.runs());
        const auto statistics = Statistics::measure(name, *search, problem, configuration.runs());
        std::cout << "done.\n";

        auto solutionFile = inputFile;
        solutionFile.replace_filename( + ".solution");
        std::cout << "Recording results for " << name << " - ";
        writeResults(statistics, inputFile, configuration.statsOutfile());
        std::cout << "written.\n";
    }

    std::cout << "All done.\n";
}
