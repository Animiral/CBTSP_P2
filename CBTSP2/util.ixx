/**
 * Provides independent, self-contained utility and helper functions.
 */
module;

#include <string>
#include <ranges>
#include <numeric>
#include <type_traits>
#include <stdexcept>
#include <cassert>

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
 * Join the given string-like objects with the given separator.
 *
 * @param separator: string to appear in-between all others
 * @param strings: range of strings
 * @return: the combined string
 */
export template <typename Separator, typename Strings>
std::string join(const Separator& separator, const Strings& strings)
{
    assert(!strings.empty());

    std::string result;
    const auto strSize = std::accumulate(
        std::ranges::cbegin(strings), std::ranges::cend(strings),
        0ull, [](std::size_t sum, const auto& s) { return sum + s.size(); });
    result.reserve((std::size(strings) - 1) * std::size(separator) + strSize + 1);

    result.append(*std::ranges::begin(strings));
    for (const auto& s : std::ranges::subrange(++std::ranges::begin(strings), std::ranges::end(strings))) {
        result.append(separator);
        result.append(s);
    }
    return result;
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
