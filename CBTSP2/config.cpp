module;

#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

module config;

import util;

// Amateur hour options parsing
struct Parser
{
    int argc;
    const char** argv;

    bool end() const
    {
        return argc <= 0;
    }

    const char* next()
    {
        if(end())
            throw std::out_of_range("Command line unexpectedly short.");

        argc--;
        return *++argv;
    }

    enum class Token { LITERAL, ALGORITHM, STEP, ITERATIONS, POPSIZE, RUNS, STATS_OUT };

    Token what() const
    {
        using namespace std::string_literals;

        assert(!end());
        const auto opt = argv[0];

        if ("-a"s == opt || "--algorithm"s == opt)  return Token::ALGORITHM;
        if ("-s"s == opt || "--step"s == opt)       return Token::STEP;
        if ("-i"s == opt || "--iterations"s == opt) return Token::ITERATIONS;
        if ("-p"s == opt || "--popsize"s == opt)    return Token::POPSIZE;
        if ("-r"s == opt || "--runs"s == opt)       return Token::RUNS;
        if ("-d"s == opt || "--dump"s == opt)       return Token::STATS_OUT;

        return Token::LITERAL;
    }

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

    Configuration::StepFunction stepFunction()
    {
        using namespace std::string_literals;

        const auto opt = next();

        if ("random"s == opt)            return Configuration::StepFunction::RANDOM;
        if ("first-improvement"s == opt) return Configuration::StepFunction::FIRST_IMPROVEMENT;
        if ("best-improvement"s == opt)  return Configuration::StepFunction::BEST_IMPROVEMENT;

        throw std::out_of_range("Unknown step function: "s + opt);
    }

    int intArg(int minValue = 1)
    {
        int value = std::stoi(next());

        if (value < minValue)
            throw std::out_of_range(format("Integer argument value too small: {} (< {})", value, minValue));

        return value;
    }

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
        case Parser::Token::RUNS:         runs_ = parser.intArg(); break;
        case Parser::Token::STATS_OUT:    statsOutfile_ = parser.pathArg(); break;
        default: assert(0);

        }

        token = parser.next();
    }
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

int Configuration::runs() const noexcept
{
    return runs_;
}

std::filesystem::path Configuration::statsOutfile() const noexcept
{
    return statsOutfile_;
}

const std::vector<std::filesystem::path>& Configuration::inputFiles() const noexcept
{
    return inputFiles_;
}
