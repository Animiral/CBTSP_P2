/**
 * Contains infrastructure for reading program configuration from program arguments and sources.
 */
module;

#include <vector>
#include <filesystem>
#include <exception>

export module config;

import mco;

using InputFiles = std::vector<std::filesystem::path>; //!< Type of input files list

/**
 * Contains all collected and parsed settings for executing a program run.
 */
export class Configuration
{

public:

    /**
     * Parse configuration from program arguments.
     */
    void readArgv(int argc, const char* argv[]);

    /**
     * Enumeration of available preset run suites, which cover multiple configurations.
     * to run as the main mode of the program.
     */
    enum class Suite { SINGLE, BENCH_MCO, POPSIZE_MCO };

    /**
     * Enumeration of available heuristics to run as the main mode of the program.
     */
    enum class Algorithm { DET_CONSTRUCTION, RAND_CONSTRUCTION, LOCAL_SEARCH, GRASP, VND, MCO };

    /**
     * Enumeration of available step functions to use in local search.
     */
    enum class StepFunction { RANDOM, FIRST_IMPROVEMENT, BEST_IMPROVEMENT };

    Suite suite = Suite::SINGLE; //!< run preset
    Algorithm algorithm = Algorithm::GRASP; //!< main search mode
    StepFunction stepFunction = StepFunction::BEST_IMPROVEMENT; //!< step strategy for local search
    int iterations = 100; //!< number of iterations for GRASP and MCO
    int popsize = 100; //!< MCO: number of mice
    float evaporation = .1f; //!< MCO: fraction of pheromone decrease per tick
    float elitism = 1.f; //!< MCO: factor of pheromone contribution of best solution so far
    float pheromoneAttraction = 10.f; //!< MCO: to which degree local pheromones attract
    float objectiveAttraction = 1.f; //!< MCO: to which degree local objective value attracts
    ReinforceStrategy reinforceStrategy = ReinforceStrategy::LAMARCK; //!< MCO: pheromone update source
    int runs = 100; //!< number of search attempts for statistical samples
    std::filesystem::path statsOutfile; //!< output file for statistical results
    InputFiles inputFiles; //!< CBTSP problem instance files

private:

    /**
     * Check whether the given path points to a file that we can read.
     * If any files do not exist, throw an `input_file_error`.
     */
    void validateInputFiles() const;

};

/**
 * This error occurs when input files given to the configuration do not point
 * to existing files on the file system.
 */
export class input_files_error : public std::exception
{

public:

    /**
     * Construct the error and remember the problematic files.
     */
    explicit input_files_error(const InputFiles& nonFiles);

    /**
     * Get an error message that names the problematic files.
     */
    virtual char const* what() const noexcept override;

private:

    InputFiles nonFiles_;
    std::string what_;

    std::string buildWhat(const InputFiles& nonFiles);

};
