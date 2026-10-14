# Hardware Consultancy Case Study and Matrix Multiplication Benchmark

A hardware consultancy project for a fictional client, with a C++ benchmark that tests the proposed solution.

The repository has two connected parts:

- **Part A, the consultancy:** diagnose a performance problem, design a replacement hardware system within a
  budget, and justify every component.
- **Part B, the benchmark:** a terminal C++ program that measures whether the proposed hardware removes the
  bottlenecks that were diagnosed.

The full written report is in [`report/Hardware_Consultancy_Report.pdf`](report/Hardware_Consultancy_Report.pdf).

## The scenario

**Client:** Bellerman Molecular Research Centre (fictional), a university-affiliated laboratory that simulates how
protein chains fold and interact. Its work depends on very large matrix multiplications.

**Problem:** After moving to new multi-threaded simulation software (ProteinSim 4), the simulations did not get
any faster, and a research-grant deadline was at risk. The lab's IT team blamed too little RAM and wanted to
spend a large part of the budget on memory.

**Constraints:** a budget of EUR 7,000 (excluding VAT), 3-4 weeks to deliver, compatibility with the existing
software, and reuse of existing parts where possible.

## Diagnosis

The current machine is an Intel Pentium 4 (single core, 2.8 GHz) with 4 GB of DDR RAM, 16 KB of L1 cache,
1 MB of L2 and no L3 cache, running Windows XP.

- **RAM was not the problem.** The largest matrices used today take only about 2.4% of the installed memory.
- **The processor had a single core.** No software, however well written, can run in parallel on one core,
  so the new multi-threaded software could not help.
- **The cache was tiny.** Almost every matrix operation had to go to the main memory.

## Proposed solution

| Component | Current | Recommended |
|-----------|---------|-------------|
| Processor | Pentium 4, 1 core | AMD Ryzen Threadripper PRO 5965WX, 24 cores / 48 threads, 128 MB L3 cache |
| Memory | 4 GB DDR, 2 channels | 64 GB DDR4-3200 ECC, 8 channels (8 x 8 GB) |
| Storage | 80 GB hard disk | 2 x Samsung 990 PRO 1 TB NVMe in RAID 1 |
| Graphics | Integrated GMA 900 | NVIDIA RTX A400 (display only) |
| Motherboard | LGA775 | ASUS Pro WS WRX80E-SAGE SE WIFI |
| Power, case, cooling | 430 W PSU | Corsair RM850x, Phanteks Enthoo Pro 2 Server, Noctua NH-U14S TR4-SP3 |
| Operating system | Windows XP | Windows 11 Pro |
| Extras | - | UPS to protect long calculations from power cuts, new peripherals |

Estimated total cost: **EUR 6,375-6,475**, about 90% of the budget. The report also covers maintenance,
an alternative architecture (a GPU-accelerated design) and why it was not chosen.

## Part B: the benchmark

The program multiplies two square matrices (A x B = C) in four ways, times each, and compares them:

| # | Version | What changes compared with the previous one |
|---|---------|---------------------------------------------|
| 1 | Single-threaded | Baseline: three nested loops on one thread |
| 2 | Multi-threaded | The rows of C are divided between the threads |
| 3 | Multi-threaded, memory-optimised | Loop order changed so matrix B is read along rows, not down columns |
| 4 | Multi-threaded, memory-optimised and cache-blocked | Matrices processed in 64 x 64 tiles (about 32 KB) that stay in the cache |

This tests the two diagnosed bottlenecks: version 2 isolates the effect of the **cores**, and versions 3 and 4
isolate the effect of the **cache**. A checksum (the sum of all elements of C) is compared across the four
versions to confirm that every version computes the same result.

For each multi-threaded version the program prints the **speedup** (single-thread time / multi-thread time)
and the **parallel fraction P** from Amdahl's Law, P = (1 - 1/S) / (1 - 1/N), as a percentage.

### Results

Matrix size 2000 x 2000, 24 threads, average of 5 runs on a MacBook Pro (Apple M4 Max, 16 cores, 64 GB RAM):

| Version | Average time | Speedup |
|---------|--------------|---------|
| Single-threaded | 6.10 s | 1.00x |
| Multi-threaded | 0.559 s | 10.92x |
| Multi-threaded, memory-optimised | 0.101 s | 60.58x |
| Multi-threaded, memory-optimised and cache-blocked | 0.079 s | 78.10x |

- The measured speedup from threads (10.92x) is close to the 11.2x predicted by Amdahl's Law for 24 cores, and the
  measured parallel fraction (94.8%) is close to the 95% assumed in the design.
- Cache-friendly memory access gave a further 5.5x, and cache blocking another 1.28x.
- P is only meaningful for version 2. Versions 3 and 4 also change the memory access, so their speedup includes
  more than the effect of the threads.
- **Limitations:** the test machine is not the proposed hardware (16 cores instead of 24, a different
  architecture), the old machine could not be measured, and matrix multiplication stands in for the real
  software. See the report for the full list.

## Files

| File | Purpose |
|------|---------|
| `main.cpp` | User input, timing, checksums, printing of results and laws |
| `mat_operations.h` | Header that declares all shared functions |
| `matrix_creat.cpp` | Fills a matrix with random values (fixed seeds, so every run uses the same data) |
| `multi_single_thread.cpp` | Version 1: single-threaded multiplication |
| `multi_multi_thread.cpp` | Version 2: multi-threaded multiplication |
| `multi_reorder_multi_thread.cpp` | Version 3: multi-threaded with reordered loops |
| `multi_reorder_blocked_multi_thread.cpp` | Version 4: multi-threaded, reordered and cache-blocked |
| `laws_calculations.cpp` | Speedup and Amdahl's Law parallel fraction |
| `Makefile` | Build instructions |
| `report/` | The full written report |

## Build and run

Requirements: a C++20 compiler (the Makefile uses `clang++`) and `make`. Tested on macOS (Apple Silicon).

```
make run
```

Other commands:

```
make          # build only (output: build/main)
make clean    # delete the build folder
```

To use a different compiler, change the `CXX` line at the top of the `Makefile` (for example to `g++`).

Example session:

```
Enter matrix size (e.g. 1000, 1500 or 2000): 2000
Enter number of threads: 24
```

The program then prints the time and checksum of each version, the checksum result, and a block for each
multi-threaded version with its time, speedup and parallel fraction. Invalid input (0) is replaced by a default
of 1000 for the size and 1 for the thread count, and a thread count larger than the matrix size is reduced to
the matrix size.

## Notes on measuring

- Only the multiplication itself is timed. Creating the matrices and printing are not included.
- Close other applications, plug the laptop in, and repeat the run several times, because timings vary slightly
  between runs.
- If the thread count is higher than the number of cores, the threads take turns on the cores and the parallel
  fraction looks lower than it really is.
- The tile size (64) is set at the top of `multi_reorder_blocked_multi_thread.cpp` and can be changed to test
  other sizes.
