# Solver for the Cost-Balanced Traveling Salesperson Problem

This solver exists on [GitHub](https://github.com/Animiral/CBTSP_P2).

## Options

* `--suite <single|bench-mco|popsize-mco>` run preset (default: single)
* `-a, --algorithm <det-construction|rand-construction|local-search|grasp|vnd|mco>` main search mode (default: grasp)
* `-s, --step <random|first-improvement|best-improvement>` step strategy for local search (default: best-improvement)
* `-i, --iterations N` run for N iterations for GRASP or N iterations without improvement for MCO (default: 100)
* `-p, --popsize N` MCO: use N mice (default: 100)
* `--evaporation V` MCO: pheromones everywhere revert by fraction V per tick (default: 0.1)
* `--elitism V` MCO: best solution so far contributes V times the pheromones (default: 1)
* `--min-pheromone V` MCO: minimum pheromone value (default: 0)
* `--max-pheromone V` MCO: maximum and initial pheromone value (default: 1)
* `--pheromone-attraction V` MCO: local pheromones attract to the power of V (default: 1)
* `--intensification V` MCO: chance of choosing best step (default: 0.5)
* `--objective-attraction V` MCO: local objective value attracts to the power of V (default: 1)
* `--reinforce-strategy <darwin|lamarck>` MCO: pheromone update source (default: lamarck)
* `-r, --runs N` make N search attempts for statistical samples (default: 100)
* `-d, --dump FILE` output statistical results to FILE (default: no stats output)

All non-option arguments are interpreted as input problem files.

## Output

The best solution of every invocation is written to the *solution file* for the instance.
e.g. the solution for `0020.txt` is written to `0020.solution`.

If the solution file already exists, it will be overwritten.
This can easily happen if the program is invoked multiple times or if an invocation triggers more than one statistics-gathering run (running suites).

The program prints its activity to standard output.

## Sample Invocations

Gather basic results over all instances:

```
CBTSP2-Main.exe --algorithm mco --dump mco-stats.csv instances/0*.txt
```

Run the MCO Benchmark for raw and refined variants:

```
CBTSP2-Main.exe --suite bench-mco --dump bench-mco-stats.csv instances/0010.txt instances/0015.txt instances/0020.txt instances/0025.txt instances/0030.txt instances/0040.txt instances/0050.txt
```

Run the Popsize Experiment for raw and refined variants:

```
CBTSP2-Main.exe --suite popsize-mco --dump popsize-mco-stats.csv instances/0040.txt
```
