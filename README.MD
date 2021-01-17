# Solver for the Cost-Balanced Traveling Salesperson Problem

## Options

* `--suite <single|bench-mco|popsize-mco>` run preset (default: single)
* `-a, --algorithm <det-construction|rand-construction|local-search|grasp|vnd|mco>` main search mode (default: grasp)
* `-s, --step <random|first-improvement|best-improvement>` step strategy for local search (default: best-improvement)
* `-i, --iterations N` run for N iterations for GRASP or N iterations without improvement for MCO (default: 100)
* `-p, --popsize N` MCO: use N mice (default: 100)
* `--evaporation V` MCO: pheromones everywhere decrease by fraction V per tick (default: 0.1)
* `--elitism V` MCO: best solution so far contributes V times the pheromones (default: 1)
* `--pheromone-attraction V` MCO: local pheromones attract to the power of V (default: 10)
* `--objective-attraction V` MCO: local objective value attracts to the power of V (default: 1)
* `--reinforce-strategy <darwin|lamarck>` MCO: pheromone update source (default: lamarck)
* `-r, --runs N` make N search attempts for statistical samples (default: 100)
* `-d, --dump FILE` output statistical results to FILE (default: no stats output)

## Sample Invocations

Gather basic results over all instances:

```
CBTSP2-Main.exe --algorithm mco --dump mco-stats.csv instances/0*.txt
```
