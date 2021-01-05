#include <iostream>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <ios>
#include <string>

import cbtsp;
import config;
import statistics;
import util;

// Read a problem input file
std::string readFile(std::filesystem::path filePath)
{
    auto stream = std::ifstream{ filePath };
    std::string contents;

    stream.seekg(0, std::ios::end);
    contents.reserve(narrow_cast<std::size_t>(static_cast<std::streamoff>(stream.tellg())));
    stream.seekg(0, std::ios::beg);

    contents.assign((std::istreambuf_iterator<char>(stream)),
                     std::istreambuf_iterator<char>());
    return contents;
}

Configuration theConfiguration;

int main(int argc, const char* argv[])
{
    theConfiguration.readArgv(argc, argv);

    if (theConfiguration.inputFiles().empty()) {
        std::cout << "No input file.\n";
        return {};
    }

    auto problem = Problem::fromText(readFile(theConfiguration.inputFiles().front()));
    std::cout << "Problem parsed.\n";

    auto statistics = Statistics{};
    std::cout << "Got stats.\n";
}
