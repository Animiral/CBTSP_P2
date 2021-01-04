module;

#include <stdexcept>
#include <string>
#include <cmath>
#include <limits>

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
    for (const auto& e : edges_) {
        if ((e.a == start && e.b == end) || (e.b == start && e.a == end))
            return e.value;
    }

    return big_m_;
}

void Problem::calculateBigM()
{
    // TODO: translate from Python using std::partial_sort.
    //       Also, there might be a validity problem with this calculation.

    //import heapq
    //values = [e.value for e in self.edges]
    //min_edges = heapq.nsmallest(self.vertices, values)
    //max_edges = heapq.nlargest(self.vertices, values)
    //self.big_m = sum(max_edges) - sum(min_edges) + 1
}

Problem Problem::fromText(std::string text)
{
    //auto stream = std::istringstream{ text };
    // TODO: translate from Python
    //import re

    //    raw_tokens = filter(lambda t : len(t) > 0, re.split('[ \n]+', text))
    //    raw_numbers = [int(sn) for sn in raw_tokens]

    //    if len(raw_numbers) < 2:
    //raise RuntimeError("An instance must specify the number of vertices and edges.")

    //    vertices = raw_numbers[0]
    //    edges = raw_numbers[1]

    //    if len(raw_numbers) != 2 + edges * 3:
    //raise RuntimeError("The instance must contain exactly {} edges.".format(edges))

    //    instance = Instance(vertices)

    //    for i in range(edges) :
    //        a, b, v = raw_numbers[2 + i * 3:2 + i * 3 + 3]
    //        instance.add_edge(Edge(a, b, v))

    //        # instance.calculate_big_m()
    //        return instance
    return Problem{ 3 };
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
    return ""; // TODO: translate Python
    //return " ".join(str(v) for v in self.vertices)
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
    return false; // TODO: translate Python
    //if self.is_partial() :
    //    return False

    //    for i in range(len(self.vertices)) :
    //        if self.instance.value(self.vertices[i - 1], self.vertices[i]) == self.instance.big_m :
    //            return False
    //        else :
    //            return True
}

void Solution::twoOpt(size_t v1, size_t v2)
{
    // TODO: implement
}

void Solution::normalize()
{
    //n = len(self.vertices)
    //if n < 2 :
    //    return self

    //    start = min(range(n), key = self.vertices.__getitem__)
    //    reverse = self.vertices[(start + 1) % n] > self.vertices[start - 1]

    //    if 0 == start and not reverse:
    //        return self

    //if reverse :
    //    normal_vertices = self.vertices[(start + 1) % n:] + self.vertices[:(start + 1) % n]
    //    normal_vertices.reverse()
    //else:
    //    normal_vertices = self.vertices[start:] + self.vertices[:start]

    //return Solution(self.instance, normal_vertices)
}

Value Solution::calculateValue()
{
    return 0; // TODO: translate Python
    //if len(self.vertices) <= 1 :
    //    return 0

    //    total = 0
    //    pre = self.vertices[-1]

    //    for v in self.vertices:
    //        value = self.instance.value(pre, v)
    //total += value
    //pre = v

    //return total
}
