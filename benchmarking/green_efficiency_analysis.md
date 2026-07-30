# Green Efficiency Analysis

## Measurement Methodology

The experiment used recorded execution times from two provided C programs. The comparison program measured a naive algorithm and a single-pass algorithm three times each. The naive execution times were 2.759983, 2.774566, and 2.768487 seconds. The single-pass execution times were 0.000105, 0.000106, and 0.000106 seconds. The recorded averages were 2.767679 seconds for the naive implementation and 0.000106 seconds for the single-pass implementation. A separate baseline loop was also executed three times, producing 0.227798, 0.228370, and 0.230234 seconds, with an average of approximately 0.228801 seconds.

Using three runs reduces the influence of a single unusually fast or slow execution. The analysis uses the recorded averages and also considers the spread of the measurements. The naive runs differed by 0.014583 seconds between the minimum and maximum values, while the single-pass runs differed by only 0.000001 seconds. No source code was changed during measurement.

## Observed Performance Differences

The naive algorithm was approximately 26,192.54 times slower than the single-pass algorithm, based on the recorded averages. The naive implementation required about 2.767679 seconds, while the single-pass implementation completed in about 0.000106 seconds. This corresponds to an execution-time reduction of approximately 99.996 percent for the single-pass version.

The source structure explains the measured gap. The naive implementation uses nested loops and therefore performs work that grows quadratically, described as O(n^2). The single-pass version visits each array element once, giving linear O(n) behavior. The measurements are consistent with this difference in algorithmic complexity.

However, the two implementations returned different results. The naive algorithm returned 625025000, while the single-pass algorithm returned 25000. According to the recorded notes, the naive function counts even values repeatedly inside the nested loop. Therefore, the timing comparison demonstrates a major runtime difference, but it is not a comparison between two fully equivalent computations.

## Relation Between Runtime and Energy Consumption

This experiment measured runtime, not electrical energy directly. Still, runtime is relevant to energy efficiency because a program that keeps the processor active for less time may require less total energy when executed under the same hardware and operating conditions. The single-pass implementation used dramatically less CPU time in the recorded runs, so it is the stronger candidate for lower energy use.

This conclusion must remain cautious. The experiment did not record watts, joules, CPU utilization, clock frequency, temperature, or power-management behavior. Therefore, the results support a relationship between reduced runtime and potential energy savings, but they do not prove an exact energy reduction.

## Limitations of the Experiment

The most important limitation is that the two algorithms did not produce the same result. Their runtimes cannot be treated as a perfectly controlled comparison of equivalent output. Only three runs were recorded for each test, and the measurements came from one execution environment. Background processes, system load, compiler behavior, and timer precision could affect the values. The very short single-pass runtime is also close to the resolution limits of ordinary timing methods, which can increase relative measurement uncertainty.

## Practical Engineering Takeaway

The recorded evidence shows that algorithm selection can dominate performance. Replacing repeated nested-loop work with one linear pass reduced average runtime from 2.767679 seconds to 0.000106 seconds. Engineers should first verify that implementations produce equivalent results, then compare repeated measurements under controlled conditions. For green software engineering, reducing unnecessary computation is a practical first step, but runtime measurements should be combined with direct energy measurements before making precise claims about power consumption.
