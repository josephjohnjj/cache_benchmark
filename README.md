# cache_benchmark
A benchmark to test the affects of cache in PaRSEC tasks.

All the tasks are mapped to the same node and all the tasks operate
on the same tile. The chances of a cache flush is minimized, but it
can still happen as the threads can steal from one another.

In the makefile change PARSEC_DIR to your install directory


