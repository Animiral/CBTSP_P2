module;

#include <stdexcept>
#include <string>
#include <cmath>
#include <limits>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cassert>

module cbtsp;

import util;

Problem::Problem(std::size_t vertices)
    : vertices_(vertices), big_m_(1)
{
    if (vertices < 3)
        throw std::invalid_argument("A valid instance consists of at least 3 vertices.");

    edges_.reserve(vertices);
}

std::size_t Problem::vertices() const
{
    return vertices_;
}

Value Problem::bigM() const
{
    return big_m_;
}

void Problem::addEdge(Edge edge)
{
    if (edge.a < 0 || edge.a >= vertices_)
        throw std::invalid_argument(format("Edge originates from out-of-range vertex {}.", edge.a));

    if (edge.b < 0 || edge.b >= vertices_)
        throw std::invalid_argument(format("Edge leads to out-of-range vertex {}.", edge.b));

    if (edge.a == edge.b)
        throw std::invalid_argument(format("Looping edges (vertex {}) are forbidden.", edge.a));

    if (std::numeric_limits<Value>::max() - std::abs(edge.value) < big_m_)
        throw std::overflow_error("Big-M too large.");

    edges_.push_back(edge);
    big_m_ += std::abs(edge.value);
}

Value Problem::value(Vertex start, Vertex end) const
{
    assert(start < vertices_);
    assert(end < vertices_);

    for (const auto& e : edges_) {
        if ((e.a == start && e.b == end) || (e.b == start && e.a == end))
            return e.value;
    }

    return big_m_;
}

Problem Problem::fromText(std::string text)
{
    auto stream = std::istringstream{ text };

    std::size_t vertices = 0;
    std::size_t edges = 0;

    if (stream) stream >> vertices;
    if (stream) stream >> edges;

    if (!stream)
        throw std::runtime_error("An instance must specify the number of vertices and edges.");

    auto problem = Problem{ vertices };

    for (std::size_t i = 0; i < edges; i++) {
        Vertex a; // first node in the edge
        Vertex b; // second node in the edge
        Value value; // value added to the solution that contains this edge

        if (stream) stream >> a;
        if (stream) stream >> b;
        if (stream) stream >> value;

        if (!stream)
            throw std::runtime_error(format("Failed to read edge {}.", i));

        if (a >= vertices)
            throw std::out_of_range(format("From-vertex in edge {} is out of range: {} (>= {}).", i, a, vertices));

        if (b >= vertices)
            throw std::out_of_range(format("To-vertex in edge {} is out of range: {} (>= {}).", i, b, vertices));

        problem.addEdge({ a, b, value });
    }

    return problem;
}

Solution::Solution(const Problem& problem, std::vector<Vertex>&& vertices)
    : problem_(&problem), vertices_(vertices)
{
    value_ = calculateValue();
}

Solution::Solution(const Problem& problem, std::vector<Vertex>&& vertices, Value value)
    : problem_(&problem), vertices_(vertices), value_(value)
{
}

std::string Solution::representation() const
{
    if (vertices_.empty())
        return {};

    auto appendVertex = [](std::string s, Vertex v)
    {
        s.append(" ");
        s.append(std::to_string(v));
        return s;
    };

    return std::accumulate(++vertices_.begin(), vertices_.end(),
        std::to_string(vertices_.front()), appendVertex);
}

Value Solution::objective() const noexcept
{
    return std::abs(value_);
}

bool Solution::isPartial() const noexcept
{
    return vertices_.size() < problem_->vertices();
}

bool Solution::isFeasible() const
{
    if (isPartial())
        return false;

    Vertex pre = vertices_.back();

    for (auto it = vertices_.cbegin(); it != vertices_.cend(); ++it) {
        if (problem_->value(pre, *it) >= problem_->bigM())
            return false;

        pre = *it;
    }

    return true;
}

void Solution::twoOpt(size_t v1, size_t v2)
{
    assert(v1 < vertices_.size());
    assert(v2 < vertices_.size());

    auto [low, high] = std::minmax(v1, v2);
    std::reverse(vertices_.begin() + low, vertices_.begin() + high);
}

void Solution::normalize()
{
    auto n = vertices_.size();
    if (n < 2)
        return; // single-vertex solutions are always normal

    const auto begin = vertices_.begin();
    const auto end = vertices_.end();
    std::size_t start = std::min_element(begin, end) - begin;
    bool reverse = vertices_[(start + 1) % n] > vertices_[(start + n - 1) % n];

    if (0 == start && !reverse)
        return; // already normal

    if (reverse) {
        std::rotate(begin, begin + (start + 1), end);
        std::reverse(begin, end);
    }
    else {
        std::rotate(begin, begin + start, end);
    }
}

Value Solution::calculateValue()
{
    if (vertices_.empty())
        return 0;

    Vertex pre = vertices_.back();
    Value total = 0;

    for (auto it = vertices_.cbegin(); it != vertices_.cend(); ++it) {
        total += problem_->value(pre, *it);
        pre = *it;
    }

    return total;
}
