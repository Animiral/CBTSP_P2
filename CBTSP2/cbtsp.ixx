/**
 * This module defines CBTSP Problems and accessories.
 */
module;

#include <string>
#include <cstdint>
#include <vector>

export module cbtsp;

export using Vertex = unsigned int;
export using Value = std::int64_t;

/**
 * Represents an edge with an associated move value in the CBTSP instance.
 */
export struct Edge
{
    Vertex a; //!< first node in the edge
    Vertex b; //!< second node in the edge
    Value value; //!< value added to the solution that contains this edge
};

/**
 * Represents an instance of the Cost - Balanced Traveling Salesperson Problem.
 *
 * It maintains a big-M value for unspecified edges.
 * This big-M is computed so that any Solution which contains an unspecified edge will
 * have a worse value than the worst solution which does not.
 *
 * The big-M value is computed to the smallest possible such value when
 * loading the Problem instance from its text representation.
 */
export class Problem
{

public:

    /**
     * Construct a Problem with the given number of nodes and big-M value.
     */
    explicit Problem(std::size_t vertices, Value bigM);

    /**
     * Get the number of nodes in the instance.
     */
    std::size_t vertices() const;

    /**
     * Get the value that represents no edge between vertices.
     */
    Value bigM() const;

    /**
     * Add a new edge to the instance.
     *
     * Since the graph is undirected, adding the edge(a, b) is the same as adding(b, a).
     * Multiple edges and looping edges(a, a) are forbidden.
     */
    void addEdge(Edge edge);

    /**
     * Find the move value associated with the edge between the startand end vertex.
     *
     * If there is no edge between the two vertices, return the big-M value.
     *
     * @param start: start vertex
     * @param end: end vertex
     * @return: the move value
     */
    Value value(Vertex start, Vertex end) const;

    /**
     * Parse the given text into an Instance.
     *
     * @param text: String which conforms to the syntax specified by the exercise assignment
     * @return: the Problem
     */
    static Problem fromText(std::string text);

private:

    /**
     * Calculate the big-M value for unspecified edges from the proposed edge list.
     */
    static Value calculateBigM(std::size_t vertices, const std::vector<Edge>& edges);

    std::size_t vertices_; //!< integer n of nodes in the instance, each identified by their number [0 : n - 1]
    Value big_m_; //!< value which is returned for edges between vertices that are not connected
    std::vector<Value> lookup_; //!< table of edge values

};

/**
 * Represents a (full or partial) solution to the Cost-Balanced Traveling Salesperson Problem.
 *
 * A solution is a list of vertices to visit in a CBTSP Instance in that order.
 */
export class Solution
{

public:

    /**
     * Construct a solution for the given problem using the given vertices.
     */
    explicit Solution(const Problem& problem, std::vector<Vertex>&& vertices);

    /**
     * Construct a solution for the given problem using the given vertices and value.
     *
     * @param problem: the problem instance
     * @param vertices: the full or partial solution vertex list
     * @param value: the pre-calculated solution value
     */
    explicit Solution(const Problem& problem, std::vector<Vertex>&& vertices, Value value);

    /**
     * Generate the string representation of the solution to be used in program solution output.
     *
     * @return: space - separated list of solution tour vertices
     */
    std::string representation() const;

    /**
     * Get the vector which stores the solution tour.
     *
     * @return: stored tour
     */
    const std::vector<Vertex>& vertices() const noexcept;

    /**
     * Get the tour value of this solution.
     *
     * @return: stored tour value
     */
    Value value() const noexcept;

    /**
     * Get the objective value of this solution, which can be compared for quality against other solutions.
     * In search, this should be minimized.
     *
     * @return: objective value
     */
    Value objective() const noexcept;

    /**
     * Get the tour length of the solution.
     *
     * @return: length of the stored tour
     */
    std::size_t length() const noexcept;

    /**
     * A solution is partial or incomplete if it does not visit every vertex in the instance.
     *
     * @return: true if the solution is partial, false otherwise
     */
    bool isPartial() const noexcept;

    /**
     * The Solution is feasible if it is complete and does not use “non - edges” with big-M value.
     *
     * @return: true if the Solution is feasible, false otherwise
     */
    bool isFeasible() const noexcept;

    /**
     * Get the number of edges in the Solution which have big-M value in the problem.
     *
     * @return: the number of infeasible edges
     */
    int countInfeasibleEdges() const noexcept;

    /**
     * Extend the solution by including an additional vertex at the specified position.
     *
     * @param pos: the index before which to insert the new vertex among the existing vertices
     * @param vertex: the new vertex value
     */
    void insert(std::size_t pos, Vertex vertex);

    /**
     * Compute the new value of the solution, assuming a two-edge exchange move operation.
     *
     * @param v1: the first edge to exchange is the edge leading to the vertex at this index
     * @param v2: the second edge to exchange is the edge leading to the vertex at this index
     */
    Value twoOptValue(std::size_t v1, std::size_t v2) const;

    /**
     * Perform a two-edge exchange move operation on the solution to arrive at a different solution.
     *
     * @param v1: the first edge to exchange is the edge leading to the vertex at this index
     * @param v2: the second edge to exchange is the edge leading to the vertex at this index
     */
    void twoOpt(std::size_t v1, std::size_t v2);

    /**
     * Change this solution into its normalized variant.
     *
     * Under normalization, solutions are equivalent regardless of starting pointand travel direction.
     *
     * If V_min is the smallest vertex number in the solution, rotateand reverse the solution until
     * v[0] = V_min and v[1] < v[-1].
     *
     * @return: a normalized solution object
     */
    void normalize();

private:

    const Problem* problem_;
    std::vector<Vertex> vertices_;
    Value value_;

    /**
     * Return the value of the solution.
     *
     * The value of the solution is the sum of values of all edges between each vertex in the listand
     * its successor.The successor of the last vertex is the first vertex.
     *
     * @return: the value
     */
    Value calculateValue();

};

/**
 * Interface for search algorithms and metaheuristics.
 */
export class Search
{

public:

    virtual ~Search() noexcept = default;

    /**
     * Execute the search for the given instance.
     *
     * @param problem: CBTSP instance
     * @return: the found solution
     */
    virtual Solution search(const Problem& problem) = 0;

};
