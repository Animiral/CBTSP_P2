module;

#include <vector>
#include <filesystem>

module config;

void Configuration::readArgv(int argc, const char* argv[])
{
    // TODO: implement
//parser.add_argument("-a", "--algorithm", help = "specify which heuristic to run", type = str,
//choices = ["det-construction", "rand-construction", "local-search", "grasp", "vnd"], # , "tabu"
//default = "grasp")
//parser.add_argument("-s", "--step", help = "specify which step function to use (local search, GRASP)", type = str,
//    choices = ["random", "first-improvement", "best-improvement"], default = "best-improvement")
//parser.add_argument("-i", "--iterations", help = "run GRASP until the specified number of iterations have passed",
//    type = int, default = 100)
//parser.add_argument("-r", "--runs", help = "run the algorithm the specified number of times for performance data",
//    type = int, default = 100)
//# parser.add_argument("-v", "--verbose", help = "print verbose output", action = "store_true")
//parser.add_argument("instances", help = "CBTSP instance input file(s)", type = str, nargs = '+')
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

int Configuration::runs() const noexcept
{
    return runs_;
}

const std::vector<std::filesystem::path>& Configuration::inputFiles() const noexcept
{
    return inputFiles_;
}
