module statistics;

Statistics::Statistics() noexcept
	: bestSolution_(), countInfeasible_(0), countTotal_(0)
{
}

void Statistics::submit(const Solution& solution)
{
    // TODO: translate Python
    //def submit_solution(self, solution) :
    //    self.count_total += 1

    //    if solution.is_feasible() :
    //        if self.best_solution is None or solution.objective() < self.best_solution.objective() :
    //            self.best_solution = solution
    //        else :
    //            self.count_infeasible += 1
}

const Solution* Statistics::bestSolution() const noexcept
{
    return bestSolution_ ? &*bestSolution_ : nullptr;
}

int Statistics::countInfeasible() const noexcept
{
    return countInfeasible_;
}

int Statistics::countTotal() const noexcept
{
    return countTotal_;
}
