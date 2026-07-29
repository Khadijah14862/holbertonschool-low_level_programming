# Algorithmic Efficiency Comparison

## Run 1

```text
Naive algorithm result: 625025000
Naive algorithm time: 2.759983 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000105 seconds
```

## Run 2

```text
Naive algorithm result: 625025000
Naive algorithm time: 2.774566 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000106 seconds
```

## Run 3

```text
Naive algorithm result: 625025000
Naive algorithm time: 2.768487 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000106 seconds
```

## Average Results

- Average naive algorithm time: 2.767679 seconds
- Average single-pass algorithm time: 0.000106 seconds
- Relative difference: The naive algorithm is approximately 26192.54 times slower than the single-pass algorithm.

## Interpretation

The naive implementation uses nested loops and has quadratic O(n^2) time complexity.
The single-pass implementation visits each array element once and has linear O(n) time complexity.
The single-pass implementation required substantially less CPU time in these measurements.
Runtime is used as a proxy for energy consumption; actual electrical power was not measured.

The supplied implementations return different result values because the naive function counts even values repeatedly inside its nested loop. The source code was not modified, as required.
