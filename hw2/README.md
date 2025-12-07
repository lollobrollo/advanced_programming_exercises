
# HOMEWORK 2 

For this homework, we decided to work on implementations for modules **A** (statistics module) and **C** (numerical integration module).

## CONTRIBUTIONS

In this section, contributors and respective main contributions are listed.

### Lorenzo Bortolussi, lorenzo.bortolussi03@gmail.com:
- Initial structure of project folders and CMakeLists;
- Implementation of the **Statistics module** and corresponding tests; 
- Incorporation of GetPot;
- Statystical analysis on Kaggle dataset.

### Riccardo Riccio, riccardo.riccio@studenti.units.it:
- Refinements on project structure and CMakeLists;
- Implementation of **dataset** dependancy, used to read data files and store their content to be used by the two principal modules, and corresponding tests;
- Implementation of the **Integration module** and corresponding tests.


## PROJECT SETUP
### Prerequisites:
- CMake, version > 3.10
- GTest, used for testing
- GetPot, third party library for command line parsing
- muparserx (libmuparserx-dev), third party library for parsing mathematical expressions
- Eigen3 (libeigen3-dev), third party library for linear algebra operations, in particular eigenvalues in gaussian integration

To build the libraries and executables, simply run the following commands from the root of the project

```bash
cmake -S. -Bbuild
cmake --build build
```

To perform tests on the compiled modules, switch to the build folder and type:
```bash
ctest --output-on-failure
```

## IMPLEMENTATION NOTES

### dataset module
In order to perform statistical analysis we needed to construct a data structure capable of storing the data we perform the analysis on and a way to parse the data from a file containing the dataset. The dataset structure parses csv files from the path passed in the constructor and constructs a data structure to simplify access to the data. Since our libraries do not require modification of the dataset, the structure is immutable. It implements iterators (by row) which are useful for multivariate analysis and accessors by row, column name or both overriding the operator ().

A lot of effort went into parsing the files and still some dataset with a more complex format may not load (example: datasets using quotes to define strings will include the quotes in the data cell, dataset using commas for purposes other than separating columns like for decimal separators or inside strings).

The structure of the dataset enforces columns to have elements of the same type or empty using std::optional and std::variant. This makes the structure easy to use for data analysis, but not suitable for structured data, hence the decision to not implement a json parser for this module.

The dataset was implemented both for access by column (univariate analysis) and by row (multivariate). I chose to store the dataset both as vector of columns and as vector of rows to simplify the access patterns and not to build one of the two on-the-fly as that can be expensive for bigger datasets. The choice made favoured time complexity over space complexity, which is an arbitrary choice, a more appropriate one should be done based on the constraints of the particular usecase.

### integration module
The integration module was built using the [strategy pattern](https://refactoring.guru/design-patterns/strategy/cpp/example) to implement different ways of computing weights and spacing of the integration nodes. While inheritance could allow for code reuse, the relation between the class evaluating the integral in an interval and the way the integration nodes are chosen and weighted are not in a true is-a relation. While policy-based design could be used, but it would require explicit template instanciation which would prevent a third-party to implement a different integration strategy without recompiling the library.

The muparserx library was used to parse the mathematical expression passed as a string to the integration module. The library allows to define variables and parse expressions using those variables. On Linux systems the library can be installed using the package manager (e.g. apt install libmuparserx-dev on Debian-based systems).

For the bonus part, the Eigen3 library was used to compute eigenvalues and eigenvectors of the Jacobi matrix needed for Gaussian quadrature using Golub-Welsch algorithm as described [here](https://en.wikipedia.org/wiki/Gaussian_quadrature#Golub%E2%80%93Welsch_algorithm) and [here](https://www.ams.org/journals/mcom/1969-23-106/S0025-5718-69-99647-1/S0025-5718-69-99647-1.pdf).

Tests were implemented for all the integration methods, using known integrals. The precision degree (degree up to which the method integrates polynomials exactly) of each method was used to verify the correctness of the implementation using polynomials of increasing degree.

### statistics module

To develop this module, I made some research into visitors ([here](https://www.cppstories.com/2018/06/variant/) and [here](https://www.cppstories.com/2018/09/visit-variants/)) to understand how to interact with the values contained in the dataset, to be able to do some statistical magic on them. During this research, I stumbled upon [decltype](https://stackoverflow.com/questions/18815221/what-is-decltype-and-how-is-it-used) to inspect the type of an object during compile time and [decay_t](https://en.cppreference.com/w/cpp/types/decay.html) to make type comparisons using [is_same_v](https://en.cppreference.com/w/cpp/types/is_same.html) simpler.

I decided to use constexpr to compute the result of type comparisons during compile time, as the type of the variant is defined at compile time with the template, so that the compiler could avoid compiling the unused if branch, saving time and avoiding possible type mismatch errors.

While searching ways to compute the mean of a vector, I found [this page](https://riptutorial.com/cplusplus/example/19185/using-std--nth-element-to-find-the-median--or-other-quantiles-), which suggested to use the function nth_element() as it is more efficient than sorting the whole vector.

An interesting constraint appeared when I was waiting for the dataset to be developed, as my companion is the one who worked on it. At the beginning of the project I had to develop methods making assumptions on how to interact with the dataset. After I was provided with the header of the dataset implementation, I could adapt my code and make suggestion based on the functionalities I needed.


### main data analysis demo
The trickiest part in this section was the inclusion of the third party library [GetPot](https://sourceforge.net/projects/getpot/files/). The expectation of a single header file complete with the library implementation was not met, but thanks to the clear readme file and the examples provided, I decided to simply copy the three files with the implementation (GetPot as the interface, getpot.cpp and getpot.hpp containing the declarations and definitions) into the main folder, to be inlcuded as they are into the main statistical analysis.

We also spent a lot of time doing data cleaning to use some particular datasets found on kaggle, but in the end we opted for a simpler dataset to work with.

## ANALYSIS RESULTS

The statistical analysis module on the iris dataset ([source](https://www.kaggle.com/datasets/himanshunakrani/iris-dataset)), a dataset that is well known for its simple properties.

The analysis focuses on recreating known results regarding this dataset, to prove the correctness of the developed toolbox.

After building the executables with cmake, from the main folder, where the dataset resides, we run:
```bash
../build/main/main -f iris.csv
```

Opening the generated report, the results seem to match the expectations for this dataset. For example, the three different classes are balanced, and petal length and width are highly correlated. Interestingly, petal length is the only variable where median and mean are significantly different (median: 4.35; mean: 3.759) and it's also the variable with the highest standard deviation: this could suggest the presence of several outliers.

The main program can also be used to perform numerical integration, specifying the function to integrate, the interval, the method to use and the number of intervals. For example, to compute the integral of sin(x) from 0 to pi using Simpson's rule:
```bash
../build/main/main -m simpson -i "sin(x)" -a 0 -b 3.1416 -n 100
```

This can also be used by varying the number of intervals to observe the accuracy degree and the convergence of the method specified with the `-m` flag.