/**
 * Provides independent, self-contained utility and helper functions.
 */
module;

#include <string>
#include <type_traits>
#include <stdexcept>

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

/**
 * Checking narrowing cast for integer types from The C++ Programming Language 11.5.
 */
export template<class Target, class Source>
requires std::is_integral_v<Target> && std::is_integral_v<Source>
Target narrow_cast(Source v)
{
    auto r = static_cast<Target>(v);
    if (static_cast<Source>(r) != v)
        throw std::domain_error(format("narrow_cast({}) failed.", v));

    return r;
}
