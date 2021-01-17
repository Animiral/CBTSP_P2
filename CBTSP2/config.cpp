module;

#include <string>
#include <ranges>
#include <functional>
#include <filesystem>
#include <cassert>

module config;

import util;
import mco;

/**
 * The Parser object holds the state of the options parser in progress.
 * It offers functions to tokenize command arguments and extract argument values.
 */
struct Parser
{
    int argc; //!< number of remaining unparsed arguments
    const char** argv; //!< remaining unparsed arguments data

    /**
     * Determine whether the parser has reached the end of the command line.
     */
    bool end() const
    {
        return argc <= 0;
    }

    /**
     * Advance to the next command line argument.
     *
     * @return: the pointer to the next argument in front
     */
    const char* next()
    {
        if(end())
            throw std::out_of_range("Command line unexpectedly short.");

        argc--;
        return *++argv;
    }

    /**
     * Describes the different kinds of argument values that this parser recognizes.
     */
    enum class Token
    {
        LITERAL,
        ALGORITHM, STEP,
        ITERATIONS, POPSIZE, EVAPORATION, ELITISM,
        PHEROMONE_ATTRACTION, OBJECTIVE_ATTRACTION, REINFORCE_STRATEGY,
        RUNS, STATS_OUT, OPT_END
    };

    /**
     * Determine the token type of the current argument in front.
     */
    Token what() const
    {
        using namespace std::string_literals;

        assert(!end());
        const auto opt = argv[0];

        if ("-a"s == opt || "--algorithm"s == opt)  return Token::ALGORITHM;
        if ("-s"s == opt || "--step"s == opt)       return Token::STEP;
        if ("-i"s == opt || "--iterations"s == opt) return Token::ITERATIONS;
        if ("-p"s == opt || "--popsize"s == opt)    return Token::POPSIZE;
        if ("--evaporation"s == opt)                return Token::EVAPORATION;
        if ("--elitism"s == opt)                    return Token::ELITISM;
        if ("--pheromone-attraction"s == opt)       return Token::PHEROMONE_ATTRACTION;
        if ("--objective-attraction"s == opt)       return Token::OBJECTIVE_ATTRACTION;
        if ("--reinforce-strategy"s == opt)         return Token::REINFORCE_STRATEGY;
        if ("-r"s == opt || "--runs"s == opt)       return Token::RUNS;
        if ("-d"s == opt || "--dump"s == opt)       return Token::STATS_OUT;
        if ("--"s == opt)                           return Token::OPT_END;

        return Token::LITERAL;
    }

    /**
     * Interpret the next argument value as an algorithm specification.
     *
     * @return: the argument parsed into an Algorithm
     * @throw std::out_of_range: if the argument cannot be interpreted
     */
    Configuration::Algorithm algorithm()
    {
        using namespace std::string_literals;

        const auto opt = next();

        if ("det-construction"s == opt)  return Configuration::Algorithm::DET_CONSTRUCTION;
        if ("rand-construction"s == opt) return Configuration::Algorithm::RAND_CONSTRUCTION;
        if ("local-search"s == opt)      return Configuration::Algorithm::LOCAL_SEARCH;
        if ("grasp"s == opt)             return Configuration::Algorithm::GRASP;
        if ("vnd"s == opt)               return Configuration::Algorithm::VND;
        if ("mco"s == opt)               return Configuration::Algorithm::MCO;

        throw std::out_of_range("Unknown algorithm: "s + opt);
    }

    /**
     * Interpret the next argument value as a step function specification.
     *
     * @return: the argument parsed into a StepFunction
     * @throw std::out_of_range: if the argument cannot be interpreted
     */
    Configuration::StepFunction stepFunction()
    {
        using namespace std::string_literals;

        const auto opt = next();

        if ("random"s == opt)            return Configuration::StepFunction::RANDOM;
        if ("first-improvement"s == opt) return Configuration::StepFunction::FIRST_IMPROVEMENT;
        if ("best-improvement"s == opt)  return Configuration::StepFunction::BEST_IMPROVEMENT;

        throw std::out_of_range("Unknown step function: "s + opt);
    }

    /**
     * Interpret the next argument value as a reinforcement strategy specification.
     *
     * @return: the argument parsed into a ReinforceStrategy
     * @throw std::out_of_range: if the argument cannot be interpreted
     */
    ReinforceStrategy reinforceStrategy()
    {
        using namespace std::string_literals;

        const auto opt = next();

        if ("darwin"s == opt)  return ReinforceStrategy::DARWIN;
        if ("lamarck"s == opt) return ReinforceStrategy::LAMARCK;

        throw std::out_of_range("Unknown reinforcement strategy: "s + opt);
    }

    /**
     * Interpret the next argument value as an integer value.
     *
     * @param minValue: minimum value of the argument
     * @return: the argument parsed into an integer
     * @throw std::out_of_range: if the argument is smaller than the minValue
     */
    int intArg(int minValue = 1)
    {
        const int value = std::stoi(next());

        if (value < minValue)
            throw std::out_of_range(format("Integer argument value too small: {} (< {})", value, minValue));

        return value;
    }

    /**
     * Interpret the next argument value as a floating-point value.
     *
     * @param minValue: minimum value of the argument
     * @param maxValue: maximum value of the argument
     * @return: the argument parsed into a floating-point value
     * @throw std::out_of_range: if the argument violates minValue or maxValue
     */
    float floatArg(float minValue = std::numeric_limits<float>::lowest(),
        float maxValue = std::numeric_limits<float>::max())
    {
        const float value = std::stof(next());

        if (value < minValue)
            throw std::out_of_range(format("Floating-point argument value too small: {} (< {})", value, minValue));

        if (value > maxValue)
            throw std::out_of_range(format("Floating-point argument value too large: {} (> {})", value, maxValue));

        return value;
    }

    /**
     * Interpret the next argument value as a filesystem path.
     *
     * @return: the argument parsed into a filesystem path
     * @throw std::out_of_range: if the argument is missing
     */
    std::filesystem::path pathArg()
    {
        return { next() };
    }
};

void Configuration::readArgv(int argc, const char* argv[])
{
    Parser parser{ argc, argv };
    const char* token = parser.next();

    while (!parser.end()) {
        switch (parser.what()) {

        case Parser::Token::LITERAL:      inputFiles_.emplace_back(token); break;
        case Parser::Token::ALGORITHM:    algorithm_ = parser.algorithm(); break;
        case Parser::Token::STEP:         stepFunction_ = parser.stepFunction(); break;
        case Parser::Token::ITERATIONS:   iterations_ = parser.intArg(); break;
        case Parser::Token::POPSIZE:      popsize_ = parser.intArg(); break;
        case Parser::Token::EVAPORATION:  evaporation_ = parser.floatArg(0.f, 1.f); break;
        case Parser::Token::ELITISM:      elitism_ = parser.floatArg(0.f); break;
        case Parser::Token::PHEROMONE_ATTRACTION: pheromoneAttraction_ = parser.floatArg(); break;
        case Parser::Token::OBJECTIVE_ATTRACTION: objectiveAttraction_ = parser.floatArg(); break;
        case Parser::Token::REINFORCE_STRATEGY: reinforceStrategy_ = parser.reinforceStrategy(); break;
        case Parser::Token::RUNS:         runs_ = parser.intArg(); break;
        case Parser::Token::STATS_OUT:    statsOutfile_ = parser.pathArg(); break;
        case Parser::Token::OPT_END:
            inputFiles_.insert(inputFiles_.end(), &parser.argv[1], &parser.argv[parser.argc]);
            parser.argc = 1;
            break;
        default: assert(0);

        }

        token = parser.next();
    }

    validateInputFiles();
}

Configuration::Algorithm Configuration::algorithm() const noexcept
{
    return algorithm_;
}

Configuration::StepFunction Configuration::stepFunction() const noexcept
{
    return stepFunction_;
}

int Configuration::iterations() const noexcept
{
    return iterations_;
}

int Configuration::popsize() const noexcept
{
    return popsize_;
}

float Configuration::evaporation() const noexcept
{
    return evaporation_;
}

float Configuration::elitism() const noexcept
{
    return elitism_;
}

float Configuration::pheromoneAttraction() const noexcept
{
    return pheromoneAttraction_;
}

float Configuration::objectiveAttraction() const noexcept
{
    return objectiveAttraction_;
}

ReinforceStrategy Configuration::reinforceStrategy() const noexcept
{
    return reinforceStrategy_;
}

int Configuration::runs() const noexcept
{
    return runs_;
}

std::filesystem::path Configuration::statsOutfile() const noexcept
{
    return statsOutfile_;
}

const InputFiles& Configuration::inputFiles() const noexcept
{
    return inputFiles_;
}

void Configuration::validateInputFiles() const
{
    InputFiles nonFiles;

    const auto notFile = [](const auto& p) { return !std::filesystem::is_regular_file(p); };
    std::ranges::copy_if(inputFiles_, std::back_inserter(nonFiles), notFile);

    if (!nonFiles.empty())
        throw input_files_error(nonFiles);
}

input_files_error::input_files_error(const InputFiles& nonFiles)
    : exception(), nonFiles_(nonFiles), what_(buildWhat(nonFiles))
{
}

char const* input_files_error::what() const noexcept
{
    return what_.c_str();
}

std::string input_files_error::buildWhat(const InputFiles& nonFiles)
{
    assert(!nonFiles.empty());

    const auto asStrings = nonFiles | std::ranges::views::transform([](const auto& p) { return p.string(); });
    return "Input files not found: " + join(", ", asStrings);
}
