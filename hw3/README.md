
# HOMEWORK 2 

In this homework, we enhanced the C++ scientific computing toolbox developed for homework 2 with Python bindings and additional functionalities.

## CONTRIBUTIONS

In this section, contributors and respective main contributions are listed.

### Lorenzo Bortolussi, lorenzo.bortolussi@studenti.units.it:
- pybind11 bindings for the C++ code of hw2;
- Developement of CMake to include pybind11;
- First version of python files: modules.py (C++ functionalities into Python classes) and comparison.py (performance analysis and results accuracy);
- After code revision, added another version of the statistical analyzer;


### Riccardo Riccio, riccardo.riccio@studenti.units.it:
- Revision of the bindings, corrected unique_ptr handling;
- Revision of CMakeLists.txt, added ;
- Restructuring of python code to exploit polymorphism;
- 
- 


## PROJECT SETUP
### Prerequisites:
- CMake, version > 3.10
- GTest, used for testing
- Python3, version > 3.10
- pybind11 (pybind11-dev), version > 2.13
- SciPy, verion > 1.11.4
- muparserx (libmuparserx-dev), third party library for parsing mathematical expressions
- Eigen3 (libeigen3-dev), third party library for linear algebra operations, in particular eigenvalues in gaussian integration

To build the shared library, run the following commands from the root of the project

```bash
cmake -S. -Bbuild
cmake --build build
cmake --install build
```

To perform tests on the compiled modules, switch to the build (CMakeFiles) folder and type:
```bash
ctest --output-on-failure
```


## PYBIND11 INTEGRATION RESULTS

After the previous setup steps have been completed, it is possible to move into the *python* folder and execute

```
python3 comparison.py
```

to show how integration using pybind works seamlessly as a python module. Below are reported the results of an analisys performed on the quality of our C++ core library.

### Statistics module comparison

In the following are reported comparisons of the statistical module we implemented in C++ with two different classes that perform the same operations implemented in python. In the first one (denoted as 'Python') we implemented manually and naively all the operations, while in the second one (denoted as 'Scientific Python') we used libraries like NumPy and SciPy to perform the analysis.

The comparisons have been confirmed to be pretty consistent among different re-runs of the script, considering a dataset of size $n=1000000$ of values extracted from the $sine$ function.

#### C++ vs BASIC PYTHON

```
--- Performance Comparison Analysis ---

Metric: MEAN
  C++:     0.000000 | Time: 0.055745s
  Python:     0.000000 | Time: 0.014597s
  Speedup: 0.26x faster using C++

Metric: MEDIAN
  C++:     0.000003 | Time: 0.106153s
  Python:     0.000003 | Time: 0.205642s
  Speedup: 1.94x faster using C++

Metric: VARIANCE
  C++:     0.500001 | Time: 0.064286s
  Python:     0.500001 | Time: 0.094774s
  Speedup: 1.47x faster using C++

Metric: CORRELATION
  C++:     1.000000 | Time: 0.104387s
  Python:     1.000000 | Time: 0.311248s
  Speedup: 2.98x faster using C++
```

We can see that, while statistical results are the same as expected, time measurements are surprisingly close. In particular, the pure Python approach performs better in computing the mean, while it performs worse in the computation of the other metrics. This comes as a surprise, as we expected the hybrid approach to achieve far better results thanks to its lower level nature.

We can assume Python seems competitive for these operations as it has already optimized functions, the GIL does not slow down a single threaded process and the C++ implementation is probably bottlenecked by memory management overhead, as we allocate and copy vectors inside each method call. This theory will be later explored with profiling.

#### C++ vs SCIENTIFIC PYTHON

```
Metric: MEAN
  C++:     0.000000 | Time: 0.055745s
  Scientific Python:     0.000000 | Time: 0.001213s
  Speedup: 0.02x faster using C++

Metric: MEDIAN
  C++:     0.000003 | Time: 0.106153s
  Scientific Python:     0.000003 | Time: 0.008358s
  Speedup: 0.08x faster using C++

Metric: VARIANCE
  C++:     0.500001 | Time: 0.064286s
  Scientific Python:     0.500001 | Time: 0.004129s
  Speedup: 0.06x faster using C++

Metric: CORRELATION
  C++:     1.000000 | Time: 0.104387s
  Scientific Python:     1.000000 | Time: 0.037160s
  Speedup: 0.36x faster using C++
```

The results abose show how python scientific libraries have been optimized for a long time and outperform greatly the package we developed. in particular, simpler operations like mean and median are performed much faster using numpy rather than our custom library.

Apart from the time required by the methods, we can confirm that our implementation yelds the correct results, which are the same as the ones yelded by the python methods.


### Integration Module

Here are provided the results of using the integration strategies, provided by our Integration Module, tested on the same function.

```
--- Integration Correctness ---
Integrating 'x^2' from 0.0 to 3.0 with 100 intervals...

Utilizing strategy MidpointRule:
C++ Result: 8.999775
Expected: 9.000000
Error: 2.25e-04

Utilizing strategy TrapezoidalRule:
C++ Result: 9.000450
Expected: 9.000000
Error: 4.50e-04

Utilizing strategy SimpsonsRule:
C++ Result: 9.000000
Expected: 9.000000
Error: 1.78e-15

Utilizing strategy GaussianQuadrature:
C++ Result: 9.000000
Expected: 9.000000
Error: 3.55e-15
```

The integration module shows pretty satisfactory results. For the quadratic function $x^2$, the Simpson’s and Gaussian strategies errors reach close to machine precision, as expected by their mathematical formulations. The Midpoint and Trapezoidal rules reach errors that are one the double of the other, as expected from the theory. This validates the integration between the Python-defined strategy objects and the C++ evaluation backend.


### Considerations on the developement

The file *comparisons.py*, as the name suggests, was born with the aim to show the successful integration of the bindings and to analyze the performance of the newly obtained library

The class *StatisticsAnalyzer* was born to group all methods related to the prformance analysis of the **Statystical Module** together, which allowed for example the use of a decorator to profile the execution time of methods from this module. During its developement it became clear that it could also be used as a wrapper for the original *StatisticalAnalyzer*, so more methods were added to turn it in both a tool for measuring performance and a tool for statistical analysis. Due to this fact, we decided to split *comparisons.py* in two files:
- ***modules.py***, where class wrapper for both modules can be accessed and imported;
- ***comparisons.py***, which imports the class defined in *integration.py* and performs the required analisys.

## TESTING AND PROFILING
