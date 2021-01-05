/**
 * Provides independent, self-contained utility and helper functions.
 */
module;

#include <string>

export module util;

/**
 * Minimally functional string formatting to substitute for std::format.
 * The standard <format> header is not yet supported.
 */
export std::string format(std::string fmt);

export template<typename Arg, typename ...Args>
std::string format(std::string fmt, Arg arg, Args... args)
{
    if (auto index = fmt.find("{}", 0); index != std::string::npos) {
        return fmt.substr(0, index) + std::to_string(arg) + format(fmt.substr(index + 2), args...);
    }
    else {
        return fmt;
    }
}
