module;

#include <limits>
#include <utility>

module local;

Neighborhood::Neighborhood(Solution&& base) noexcept
    : solution_(base)
{
}

Neighborhood::Neighborhood(const Neighborhood& rhs)
    : solution_(rhs.solution_)
{
}

Neighborhood::reference Neighborhood::operator*() const noexcept
{
    return solution_;
}

Neighborhood::pointer Neighborhood::operator->() const noexcept
{
    return &solution_;
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(Solution&& base) noexcept
    : Neighborhood(std::move(base))
{
}

TwoExchangeNeighborhood::TwoExchangeNeighborhood(const TwoExchangeNeighborhood& rhs)
    : Neighborhood(rhs)
{
}

TwoExchangeNeighborhood& TwoExchangeNeighborhood::operator++()
{
    return *this; // TODO: translate Python
//instance = base.instance
//    n = len(base.vertices)
//    for i in range(n - 2) : # first edge to cut is before vertex i
//        for j in range(i + 2, n if i > 0 else n - 1) : # second edge to cut is before vertex j
//            neighbor_vertices = base.vertices[:i]
//            neighbor_vertices += reversed(base.vertices[i:j])
//            neighbor_vertices += base.vertices[j:]
//            i_pos = base.vertices[i]
//            i_prev = base.vertices[i - 1]
//            j_pos = base.vertices[j % n]
//            j_prev = base.vertices[j - 1]
//            neighbor_value = base.value - instance.value(i_prev, i_pos) - instance.value(j_prev, j_pos) + \
//            instance.value(i_prev, j_prev) + instance.value(i_pos, j_pos)
//            yield Solution(instance, neighbor_vertices, neighbor_value)
}

NarrowTwoExchangeNeighborhood::NarrowTwoExchangeNeighborhood(Solution&& base) noexcept
    : Neighborhood(std::move(base))
{
}

NarrowTwoExchangeNeighborhood::NarrowTwoExchangeNeighborhood(const NarrowTwoExchangeNeighborhood& rhs)
    : Neighborhood(rhs)
{
}

NarrowTwoExchangeNeighborhood& NarrowTwoExchangeNeighborhood::operator++()
{
    return *this; // TODO: translate Python
//instance = base.instance
//n = len(base.vertices)
//max_l = max(int(instance.vertices / 4), 3)  # smaller tour max length
//
//for i in range(-(max_l - 1), n - (max_l - 1)) :
//    for j in range(i + 3, i + max_l + 1) :
//        # cut the loop in two : first for cutting beyond the bounds of the array, then for inside bounds.
//        if (i < 0) and (j >= 0) :
//            neighbor_vertices = base.vertices[j:i]
//            neighbor_vertices += reversed(base.vertices[i:] + base.vertices[:j])
//            i_pos = base.vertices[i]
//            i_prev = base.vertices[i - 1]
//            j_pos = base.vertices[j % n]
//            j_prev = base.vertices[j - 1]
//            neighbor_value = base.value - instance.value(i_prev, i_pos) - instance.value(j_prev, j_pos) + \
//                                instance.value(i_prev, j_prev) + instance.value(i_pos, j_pos)
//            yield Solution(base.instance, neighbor_vertices, neighbor_value)
//        else:
//            neighbor_vertices = base.vertices[:i]
//            neighbor_vertices += reversed(base.vertices[i:j])
//            neighbor_vertices += base.vertices[j:]
//            i_pos = base.vertices[i]
//            i_prev = base.vertices[i - 1]
//            j_pos = base.vertices[j % n]
//            j_prev = base.vertices[j - 1]
//            neighbor_value = base.value - instance.value(i_prev, i_pos) - instance.value(j_prev, j_pos) + \
//                                instance.value(i_prev, j_prev) + instance.value(i_pos, j_pos)
//            yield Solution(base.instance, neighbor_vertices, neighbor_value)
}

WideTwoExchangeNeighborhood::WideTwoExchangeNeighborhood(Solution&& base) noexcept
    : Neighborhood(std::move(base))
{
}

WideTwoExchangeNeighborhood::WideTwoExchangeNeighborhood(const WideTwoExchangeNeighborhood& rhs)
    : Neighborhood(rhs)
{
}

WideTwoExchangeNeighborhood& WideTwoExchangeNeighborhood::operator++()
{
    return *this; // TODO: translate Python
//instance = base.instance
//n = len(base.vertices)
//min_l = max(int(instance.vertices / 4), 3) + 1  # smaller tour min length
//max_l = int(instance.vertices / 2)  # smaller tour max length
//
//for i in range(-(max_l - 1), n - (max_l - 1)):
//    for j in range(i + min_l, i + max_l + 1):
//        # cut the loop in two: first for cutting beyond the bounds of the array, then for inside bounds.
//        if (i < 0) and (j >= 0):
//            neighbor_vertices = base.vertices[j:i]
//            neighbor_vertices += reversed(base.vertices[i:] + base.vertices[:j])
//            i_pos = base.vertices[i]
//            i_prev = base.vertices[i-1]
//            j_pos = base.vertices[j % n]
//            j_prev = base.vertices[j-1]
//            neighbor_value = base.value - instance.value(i_prev, i_pos) - instance.value(j_prev, j_pos) +\
//                                instance.value(i_prev, j_prev) + instance.value(i_pos, j_pos)
//            yield Solution(base.instance, neighbor_vertices, neighbor_value)
//        else:
//            neighbor_vertices = base.vertices[:i]
//            neighbor_vertices += reversed(base.vertices[i:j])
//            neighbor_vertices += base.vertices[j:]
//            i_pos = base.vertices[i]
//            i_prev = base.vertices[i-1]
//            j_pos = base.vertices[j % n]
//            j_prev = base.vertices[j-1]
//            neighbor_value = base.value - instance.value(i_prev, i_pos) - instance.value(j_prev, j_pos) +\
//                                instance.value(i_prev, j_prev) + instance.value(i_pos, j_pos)
//            yield Solution(base.instance, neighbor_vertices, neighbor_value)
}

VertexShiftNeighborhood::VertexShiftNeighborhood(Solution&& base) noexcept
: Neighborhood(std::move(base))
{
}

VertexShiftNeighborhood::VertexShiftNeighborhood(const VertexShiftNeighborhood& rhs)
    : Neighborhood(rhs)
{
}

VertexShiftNeighborhood& VertexShiftNeighborhood::operator++()
{
    return *this; // TODO: translate Python
//instance = base.instance
//n = len(base.vertices)
//for i in range(n):
//    i_prev = (i+n-1) % n  # positive index of vertex before i
//    i_pp = (i+n-2) % n  # positive index of vertex before vertex before i
//    i_next = (i+1) % n  # positive index of vertex after i
//    neighbor_value = base.value -\
//                        instance.value(base.vertices[i_pp % n], base.vertices[i_prev % n]) -\
//                        instance.value(base.vertices[i % n], base.vertices[i_next % n]) +\
//                        instance.value(base.vertices[i_pp % n], base.vertices[i % n]) +\
//                        instance.value(base.vertices[i_prev % n], base.vertices[i_next % n])
//    neighbor_vertices = base.vertices[:]
//    neighbor_vertices[i-1], neighbor_vertices[i] = neighbor_vertices[i], neighbor_vertices[i-1]
//    yield Solution(base.instance, neighbor_vertices, neighbor_value)
}

Step::Step(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : neighborhood_(move(neighborhood))
{
}

FirstImprovement::FirstImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : Step(move(neighborhood))
{
}

void FirstImprovement::step(Solution& base)
{
    // TODO: translate Python
    //v = self.objective(base)
    //return next(filter(lambda n : self.objective(n) < v, self.neighborhood(base)), base)
}

BestImprovement::BestImprovement(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : Step(move(neighborhood))
{
}

void BestImprovement::step(Solution& base)
{
    // TODO: translate Python
    //best = min(self.neighborhood(base), key = self.objective, default = base)
    //if self.objective(best) < self.objective(base) :
    //    return best
    //else :
    //    return base
}

StepRandom::StepRandom(std::unique_ptr<Neighborhood> neighborhood) noexcept
    : Step(move(neighborhood))
{
}

void StepRandom::step(Solution& base)
{
    // TODO: translate Python
    //ns = list(self.neighborhood(base))
    //return self.random.choice(ns) if len(ns) > 0 else base
}

WhenStagnant::WhenStagnant() noexcept
    : best_(std::numeric_limits<Value>::max())
{
}

bool WhenStagnant::doneAfter(const Solution& solution) noexcept
{
    auto objective = solution.objective();
    if (objective >= best_) {
        return true;
    }
    else {
        best_ = objective;
        return false;
    }
}

LocalSearch::LocalSearch(std::unique_ptr<Step> step, WhenStagnant doTerminate) noexcept
    : step_(move(step)), doTerminate_(doTerminate)
{
}

Solution LocalSearch::search(Solution&& start) const
{
    return Solution{ start }; // TODO: translate Python
    //while not self.done(self, start) :
    //    start = self.step(start)

    //    return start
}

StandaloneLocalSearch::StandaloneLocalSearch(
    DeterministicConstruction&& construction,
    std::unique_ptr<Step> step,
    WhenStagnant doTerminate) noexcept
    : construction_(std::move(construction)), local_(move(step), doTerminate)
{
}

Solution StandaloneLocalSearch::search(const Problem& problem)
{
    return local_.search(construction_.construct(problem));
}