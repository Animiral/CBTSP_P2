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
import mco;
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

    /**
     * Initialize the builder with the given components.
     *
     * @param algorithm: choice of search heuristic
     * @param stepFunction: choice of neighborhood step function
     * @param iterations: number of iterations for GRASP and MCO
     * @param popsize: number of mice in an iteration of MCO
     * @param random: random number generator
     */
    explicit SearchBuilder(Configuration::Algorithm algorithm,
        Configuration::StepFunction stepFunction,
        int iterations,
        int popsize,
        const std::shared_ptr<Random>& random) noexcept;

    /**
     * Construct the search object with the given parameters.
     *
     * @return: the search algorithm object
     */
    std::unique_ptr<Search> buildSearch() const;

private:

    // configurable search parameters
    Configuration::Algorithm algorithm_;
    Configuration::StepFunction stepFunction_;
    int iterations_;
    int popsize_;
    std::shared_ptr<Random> random_;

    // pre-tuned search parameters, to be exposed later
    constexpr static float evaporation_ = .1f; // MCO: fraction of pheromone decrease per tick
    constexpr static float pheromoneAttraction_ = 10.f; // MCO: to which degree local pheromones attract
    constexpr static float objectiveAttraction_ = 1.f; //  MCO: to which degree local objective value attracts
    constexpr static ReinforceStrategy reinforceStrategy_ = ReinforceStrategy::LAMARCK;

    std::unique_ptr<DeterministicConstruction> buildDeterministicConstruction() const;
    std::unique_ptr<RandomConstruction> buildRandomConstruction() const;
    std::unique_ptr<Neighborhood> buildFullNeighborhood() const;
    std::unique_ptr<Step> buildStep(std::unique_ptr<Neighborhood> neighborhood) const;
    std::vector<std::unique_ptr<Step>> buildVndSteps() const;
    std::unique_ptr<LocalSearch> buildImprovement() const;

};
