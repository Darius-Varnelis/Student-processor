# OOP-Project
## Introduction
This program was written as a project for an Object Oriented Programming course. The purpose of the program is to read or generate student data which will then be used to calculate their final grades.

## Build
The project uses CMake (3.16 or newer) and requires a C++17 compiler.
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
When using a multi-config generator (e.g. Visual Studio), build with:
```bash
cmake --build build --config Release
```
The program reads and writes data files in its working directory, so run it from the folder where you want the generated files to be stored.

## Usage
Upon running the compiled program, the user will be prompted with several options to decide the behavior of the program. 

![](images/Screenshot_Options.png)

The following formula is used for final grade calculation:
```math
Final grade = 0.4 \cdot average\space or\space median + 0.6 \cdot exam
```

## Structure
Most of the logic is written as function templates over a generic `Konteineris` type, so the same code works with both `std::vector<Studentas>` and `std::list<Studentas>`. The container is chosen once, at runtime, in `main.cpp`. Where the two containers need different code (sorting), `if constexpr` picks the right version at compile time.

### main.cpp
The program's entry point. It asks the user to choose a mode (manual entry, random generation, reading from a file, or benchmark), a sort field and order, a container, an output target and which statistic to show. It then calls the matching template function with either `std::vector<Studentas>` or `std::list<Studentas>`.

### studentas.h
- `struct Studentas`: first name, last name, homework grades, exam grade and final grade.
- `skaiciuotiGalutini()`: calculates the final grade (0.4 × homework average + 0.6 × exam).
- `rusiuotiStudentus()`: sorts by first name, last name or final grade, in ascending or descending order. Uses `std::sort` for `vector` and the member `list::sort()` for `list`.
- `skirstytiStudentusMano()`: splits students into failing (*nuskriaustukai*, final grade < 5) and passing (*kietukai*, final grade ≥ 5) groups using `std::move` to move students from the original container into two new ones, leaving the original one empty..
- `skirstytiStudentus1()`: splits students into two groups in a similar way to `skirstytiStudentusMano()`, using `push_back` instead of `std::move`.
- `skirstytiStudentus2()`: splits students into two groups by finding failing ones, moving them into a new container and erasing them from the original one. The end result is 2 containers with passing and failing students.
- `skirstytiStudentus1Partition()`: works as an improved version of `skirstytiStudentusMano()` that groups students into two new containers, first partitioning students by passing grade using `std::partition`.
- `skirstytiStudentus2Partition()`: works as an improved version of `skirstytiStudentus2()` that partitions students by passing grade using `std::partition`, then moves the failing ones into a new container with a single `insert` and erases them from the original one with a single `erase`.

### manual.h / manual.cpp
Console interaction and the non-benchmark modes:
- `nustatymai()`, `ivestiSk()`, `taipArNe()`: menu prompts and validated number and yes/no input.
- `ivestiRanka()`: manual entry, with an option to generate grades randomly. After each student is added, it prints the student's address in memory.
- `nuskaitytiIsFailo()`: reads, processes and outputs students from an existing file.
- `rasytiAntraste()`, `rasytiStudenta()`, `rasytiGrupe()`: write the formatted results table to the console or a file.

### failai.h / failai.cpp
File I/O:
- `generuotiFaila()`: generates a test file with *n* random students. The whole file is built in a `stringstream` first and written to disk in one go.
- `nuskaitytiStudentus()`: reads a student file into any container.
- `isvestiStudentus()`: writes a group of students to a file.
- `arFailaiEgzistuoja()`: checks whether the benchmark files already exist, so they don't have to be regenerated.

### testai.h / testai.cpp
The benchmark. `testas()` times each step (read, final grade calculation, sort, grouping, writing both groups) on one file. `testuPaleistuvas()` repeats the test 5 times per file for the chosen container and averages the results. `paleistiAtmintiesTestus()` runs the memory test by launching the program again as a separate process for every grouping method, so that the peak memory resets between tests. `vienasTestas()` is the test run inside that process: it reads the students, groups them with the chosen method and outputs how much peak memory grew during grouping.

### results.h / results.cpp
`struct Rezultatai` stores the average time for each step. `spausdintiLentele()` exports all results as a Markdown table, which is where the tables in this README come from.

### nustatymai.h
The `enum class` types that hold the program's settings: `Rezimas`, `Struktura`, `Rusiuoti`, `Tvarka`, `Statistika` and `Isvedimas`.

### timer.h
A small `Timer` class built on `std::chrono::high_resolution_clock`, with `reset()` and `elapsed()`.

### utils.h / utils.cpp
Helpers:
- `vidurkis()`, `mediana()`: calculate the average and the median of the homework grades.
- `utf8Plotis()`: corrects `setw` column width for Lithuanian UTF-8 characters.
- `pikineAtmintis()`: returns the peak memory used by the program so far, in bytes. Used by the memory test to measure how much the peak grew during grouping. On Windows it uses `GetProcessMemoryInfo`, and on Linux it reads `VmHWM` from `/proc/self/status`.
- `dabartineAtmintis()`: returns how much memory the program is using right now, in bytes. Works the same way as `pikineAtmintis()`, but reads `VmRSS` on Linux.

### kursiokai.txt
A small sample student file for testing the read-from-file mode.


## Benchmarking
Upon running the app, users have the option to select a benchmarking mode. During the benchmarking mode the program randomly generates 5 files with varying amounts of students which will then be used to test the program's performance on analyzing them. Students are put in a failing group *(nuskriaustukai)* if their grade is lower than 5 and a passing group *(kietukai)* if their grade is 5 or greater.
All steps are timed and repeated 5 times, the average time for each step is calculated and output to the console.

The goal of the benchmark is to compare performance between the Struct and Class versions, as well as the performance of optimizer flags.
Vector and List containers in this app and to decide which one fits the program better.
![](images/Screenshot_Benchmark.png)

## Benchmark Results

### Struct vs Class Comparison

| Implementation | Student count | Average file read time | Average final grade calculation time | Average sorting time | Average grouping time | Average failed student output time | Average passing student output time | Average processing time |
|---|---|---|---|---|---|---|---|---|
| **Struct** | 100,000 | 0.164s | 0.001s | 0.027s | 0.002s | 0.022s | 0.023s | 0.240s |
| **Class** | 100,000 | 0.164s | 0.001s | 0.028s | 0.003s | 0.024s | 0.024s | 0.244s |
| **Struct** | 1,000,000 | 1.620s | 0.011s | 0.268s | 0.023s | 0.279s | 0.283s | 2.484s |
| **Class** | 1,000,000 | 1.668s | 0.011s | 0.281s | 0.032s | 0.290s | 0.292s | 2.574s |

### Compiler Optimization Comparison (-O1, -O2, -O3)

##### -O1 (Executable size: 115.2 KiB)

| Student count | Average file read time | Average final grade calculation time | Average sorting time | Average grouping time | Average failed student output time | Average passing student output time | Average processing time |
|---|---|---|---|---|---|---|---|
| 100,000 | 0.163s | 0.002s | 0.027s | 0.003s | 0.022s | 0.023s | 0.239s |
| 1,000,000 | 1.678s | 0.016s | 0.287s | 0.026s | 0.305s | 0.310s | 2.622s |

##### -O2 (Executable size: 117.0 KiB)

| Student count | Average file read time | Average final grade calculation time | Average sorting time | Average grouping time | Average failed student output time | Average passing student output time | Average processing time |
|---|---|---|---|---|---|---|---|
| 100,000 | 0.172s | 0.001s | 0.032s | 0.003s | 0.027s | 0.024s | 0.260s |
| 1,000,000 | 1.631s | 0.011s | 0.293s | 0.026s | 0.287s | 0.294s | 2.542s |

##### -O3 (Executable size: 139.6 KiB)

| Student count | Average file read time | Average final grade calculation time | Average sorting time | Average grouping time | Average failed student output time | Average passing student output time | Average processing time |
|---|---|---|---|---|---|---|---|
| 100,000 | 0.164s | 0.001s | 0.028s | 0.003s | 0.024s | 0.024s | 0.244s |
| 1,000,000 | 1.668s | 0.011s | 0.281s | 0.032s | 0.290s | 0.292s | 2.574s |

### Testing Conditions
Test system:
- CPU: AMD Ryzen AI 9 HX 370 (12 cores / 24 threads)
- RAM: 32 GB LPDDR5X
- Storage: NVMe SSD
- OS: Linux (CachyOS, kernel 7.3.3)
- Compiler: GCC 16.2.1
- Power: AC (plugged in), Balanced profile
- Results: Average of 5 runs on pre-generated files
- Files were generated once per build
- Optimizations used: -O1, -O2, -O3
- Sorted by final grade descending, after grouping

### Benchmark Conlusions

#### Struct vs Class



## Disclaimer
The generation of the text files is likely to take a large amount of time (about 30 seconds) as well as a large amount of storage (about 5GB). Further processing of the files (5 runs for every file) takes about 9 minutes on the release build.

![](images/Screenshot_Sizes.png)

## Releases

### [v1.0](https://github.com/Darius-Varnelis/OOP-Project/releases/tag/v1.0)
- Added four more strategies for splitting students into passing and failing groups, next to the original `std::move` method:
  - **Method 1** (`skirstytiStudentus1`) copies students into two new containers.
  - **Method 2** (`skirstytiStudentus2`) moves failing students into a new container and removes them from the original one by moving the last element into the gap and calling `pop_back()`.
  - **Method 1 with partition** (`skirstytiStudentus1Partition`) groups students with `std::partition`, then bulk-moves each group into a new container with `insert` and `std::make_move_iterator`.
  - **Method 2 with partition** (`skirstytiStudentus2Partition`) groups students with `std::partition`, then bulk-moves the failing ones into a new container and removes them from the original one with a single `erase`.
- Added timing for all grouping methods to the benchmark, for both `vector` and `list`.
- Added peak memory measurement during grouping. Each test runs in a separate process so the peak counter resets between methods.
- The vector/list choice now applies to every container in the program.
- Added an option to delete the generated benchmark files.
- README: added a grouping method comparison (speed and memory) and updated the benchmark results.

### [v0.3](https://github.com/Darius-Varnelis/OOP-Project/releases/tag/v0.3)
- Added `std::list` as an alternative to `std::vector`, chosen at runtime. All logic was rewritten as templates over the container.
- Added a vector vs list benchmark that runs both containers right away.
- Added a sorting order option (ascending/descending).
- Manual entry now supports sorting, random grade generation and writing the results to a file.
- Added an option to read student data from an existing file.
- Moved the test logic and settings enums into separate files.
- Fixed an out-of-memory bug and added error handling for invalid input.
- README: added a program structure overview, images and a vector vs list analysis.

### [v0.2](https://github.com/Darius-Varnelis/OOP-Project/releases/tag/v0.2)
- Added random test file generation. The whole file is built in a `stringstream` before being written to disk.
- Added splitting students into failing (*nuskriaustukai*) and passing (*kietukai*) groups, each written to its own file.
- Added a `Timer` class and timing of each step, repeated 5 times per file and averaged.
- Added sorting by a user-chosen field.
- Benchmark results are exported as a Markdown table.
- Split `main.cpp` into separate source and header files, and switched to consistent naming (camelCase functions, PascalCase structs).
- Added CMake build support.

### [v0.1](https://github.com/Darius-Varnelis/OOP-Project/releases/tag/v0.1)
- Added reading student data from a file (`kursiokai.txt`).
- Sorted output and a separator line between the header and the data.
- Added support for Lithuanian characters on Windows.
- Readability improvements using `constexpr` and `enum class`.

### [v.pradinė](https://github.com/Darius-Varnelis/OOP-Project/releases/tag/v.pradin%C4%971)
- First version: manual student entry with homework and exam grades.
- Final grade calculation using the average or the median of homework grades.
- Input validation, including entry of an unknown number of grades.
- An option to generate grades randomly, with a seeded random generator.
