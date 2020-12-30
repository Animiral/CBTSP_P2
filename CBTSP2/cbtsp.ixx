/**
 * This module defines CBTSP Problems and accessories.
 */
module;

#include <sstream>
#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <limits>

export module cbtsp;

import util;

/**
 * Represents an edge with an associated move value in the CBTSP instance.
 */
export struct Edge
{
    int a; //!< first node in the edge
    int b; //!< second node in the edge
    int64_t value; //!< value added to the solution that contains this edge
};

/**
 * Represents an instance of the Cost - Balanced Traveling Salesperson Problem.

 * It automatically maintains a big-M value for unspecified edges.
 * This big-M is computed so that any Solution which contains an unspecified edge will
 * have a worse value than the worst solution which does not.
 * The big-M value can be recomputed to the smallest possible such value after
 * the Problem instance is complete.
 */
export class Problem
{

public:

    /**
     * Construct a Problem with the given number of nodes.
     */
    explicit Problem(int vertices)
        : vertices_(vertices), big_m_(1)
    {
        if (vertices < 3)
            throw std::invalid_argument("A valid instance consists of at least 3 vertices.");

        edges_.reserve(vertices);
    }

    /**
     * Get the number of nodes in the instance.
     */
    int vertices() const
    {
        return vertices_;
    }

    /**
     * Get the number of nodes in the instance.
     */
    int64_t bigM() const
    {
        return big_m_;
    }

    /**
     * Add a new edge to the instance.
     *
     * Since the graph is undirected, adding the edge(a, b) is the same as adding(b, a).
     * Multiple edges and looping edges(a, a) are forbidden.
     */
    void addEdge(Edge edge)
    {
        if (edge.a < 0 || edge.a >= vertices_)
            throw std::invalid_argument(format("Edge originates from out-of-range vertex {}.", edge.a));

        if (edge.b < 0 || edge.b >= vertices_)
            throw std::invalid_argument(format("Edge leads to out-of-range vertex {}.", edge.b));

        if (edge.a == edge.b)
            throw std::invalid_argument(format("Looping edges (vertex {}) are forbidden.", edge.a));

        if (std::numeric_limits<int64_t>::max() - std::abs(edge.value) < big_m_)
            throw std::overflow_error("Big-M too large.");

        edges_.push_back(edge);
        big_m_ += std::abs(edge.value);
    }

    /**
     * Find the move value associated with the edge between the startand end vertex.
     *
     * If there is no edge between the two vertices, return the big-M value.
     *
     * @param start: start vertex
     * @param end: end vertex
     * @return: the move value
     */
    int64_t value(int start, int end) const
    {
        for (const auto& e : edges_) {
            if ((e.a == start && e.b == end) || (e.b == start && e.a == end))
                return e.value;
        }

        return big_m_;
    }

    /**
     * Recalculate the big_m attribute value for unspecified edges.
     */
    void calculateBigM()
    {
        // TODO: translate from Python using std::partial_sort.
        //       Also, there might be a validity problem with this calculation.

        //import heapq
        //values = [e.value for e in self.edges]
        //min_edges = heapq.nsmallest(self.vertices, values)
        //max_edges = heapq.nlargest(self.vertices, values)
        //self.big_m = sum(max_edges) - sum(min_edges) + 1
    }

    /**
     * Parse the given text into an Instance.
     *
     * @param text: String which conforms to the syntax specified by the exercise assignment
     * @return: the Problem
     */
    static Problem fromText(std::string text)
    {
        auto stream = std::istringstream{ text };
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
        return Problem{3};
    }

private:

    int vertices_; //!< integer n of nodes in the instance, each identified by their number [0 : n - 1]
    std::vector<Edge> edges_; //!< list of Edge objects connecting vertices
    int64_t big_m_; //!< value which is returned for edges between vertices that are not connected

};
