/**
 * Contains infrastructure for reading program configuration from program arguments and sources.
 */
module;

#include <vector>
#include <filesystem>
#include <exception>

export module config;

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
     * Enumeration of available heuristics to run as the main mode of the program.
     */
    enum class Algorithm { DET_CONSTRUCTION, RAND_CONSTRUCTION, LOCAL_SEARCH, GRASP, VND, MCO };

    /**
     * Get the configured algorithm.
     */
    Algorithm algorithm() const noexcept;

    /**
     * Enumeration of available step functions to use in local search.
     */
    enum class StepFunction { RANDOM, FIRST_IMPROVEMENT, BEST_IMPROVEMENT };

    /**
     * Get the configured step function.
     */
    StepFunction stepFunction() const noexcept;

    /**
     * Get the configured number of iterations for random sampling in GRASP.
     */
    int iterations() const noexcept;

    /**
     * Get the configured number of mice for MCO.
     */
    int popsize() const noexcept;

    /**
     * Get the configured number of runs for sampling run time.
     */
    int runs() const noexcept;

    /**
     * Get the configured output file for the statistical evaluation.
     */
    std::filesystem::path statsOutfile() const noexcept;

    /**
     * Get the configured set of CBTSP instance input files.
     */
    const InputFiles& inputFiles() const noexcept;

private:

    Algorithm algorithm_ = Algorithm::GRASP;
    StepFunction stepFunction_ = StepFunction::BEST_IMPROVEMENT;
    int iterations_ = 100;
    int popsize_ = 100;
    int runs_ = 100;
    std::filesystem::path statsOutfile_;
    InputFiles inputFiles_;

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

    explicit input_files_error(const InputFiles& nonFiles);

    virtual char const* what() const noexcept override;

private:

    InputFiles nonFiles_;
    std::string what_;

    std::string buildWhat(const InputFiles& nonFiles);

};
