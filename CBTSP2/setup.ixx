/**
 * Contains infrastructure to turn the program configuration into program objects.
 *
 * Provides functions to read and write input and output files.
 */
module;

#include <filesystem>
#include <random>
#include <memory>
#include <vector>

export module setup;

import cbtsp;
import construction;
import local;
import config;
import statistics;

/**
 * Read a problem from the given input file.
 *
 * @param filePath: input file path
 * @return: problem object
 */
export Problem readProblemFile(std::filesystem::path filePath);

/**
 * Write the results of a run to the appropriate output files.
 *
 * The contents of the solution file, if it exists, will be overwritten.
 *
 * The statistics of the search will be appended as a new CSV format
 * line to the specified target file.
 *
 * @param statistics: program execution results to write
 * @param solutionPath: solution output file path
 * @param statsOutPath: statistics dump file path
 */
export void writeResults(const Statistics& statistics,
    std::filesystem::path solutionPath, std::filesystem::path statsOutPath);

/**
 * This class can wire up the correct search algorithm from the program configuration.
 */
export class SearchBuilder
{

public:

    explicit SearchBuilder(Configuration::Algorithm algorithm,
        Configuration::StepFunction stepFunction,
        const Problem& problem_,
        int iterations) noexcept;

    std::unique_ptr<Search> buildSearch() const;

private:

    Configuration::Algorithm algorithm_;
    Configuration::StepFunction stepFunction_;
    const Problem* problem_;
    int iterations_;

    std::default_random_engine buildRandom() const;
    std::unique_ptr<DeterministicConstruction> buildDeterministicConstruction() const;
    std::unique_ptr<RandomConstruction> buildRandomConstruction() const;
    std::unique_ptr<Neighborhood> buildFullNeighborhood() const;
    std::unique_ptr<Neighborhood> buildShiftNeighborhood() const;
    std::unique_ptr<Neighborhood> buildNarrowNeighborhood() const;
    std::unique_ptr<Neighborhood> buildWideNeighborhood() const;
    std::unique_ptr<Step> buildStep(std::unique_ptr<Neighborhood> neighborhood) const;
    std::vector<std::unique_ptr<Step>> buildVndSteps() const;

};
