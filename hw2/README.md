
# HOMEWORK 2 

For this homework, we decided to work on implementations for modules **A** (statistics module) and **C** (numerical integration module).


## CONTRIBUTIONS

In this section, contributors and respective main contributions are listed.

### Lorenzo Bortolussi, lorenzo.bortolussi03@gmail.com:
- Initial structure of project folders and CMakeLists;
- Implementation of the **Statistics module** and corresponding tests; 
- Incorporation of GetPot;
- Statystical analysis on Kaggle dataset.

### Riccardo Riccio, *-mail-* :
- Refinements on project structure and CMakeLists;
- Implementation of **dataset** dependancy, used to read data files and store their content to be used by the two principal modules, and corresponding tests;
- Implementation of the **Integration module** and corresponding tests.


## PROJECT SETUP
### Prerequisites:
- CMake, version > 3.10
- GTest, used for testing
- GetPot, third party library for command line parsing

To build the object files, simply run the following commands from the root of the project

```bash
cmake -S. -BCMakeFiles 
cmake --build CMakeFiles
```

To perform tests on the compiled modules, switch to the CMakeFiles folder and type:
```bash
ctest --output-on-failure
```

To create the executable files, in the build folder run:
```bash
make
```

## IMPLEMENTATION NOTES

### dataset module
...

### integration module
...

### statistics module

To develop this module, I made some research into visitors ([here](https://www.cppstories.com/2018/06/variant/) and [here](https://www.cppstories.com/2018/09/visit-variants/)) to understand how to interact with the values contained in the dataset, to be able to do some statistical magic on them. During this research, I stumbled upon [decltype](https://stackoverflow.com/questions/18815221/what-is-decltype-and-how-is-it-used) to inspect the type of an object during compile time and [decay_t](https://en.cppreference.com/w/cpp/types/decay.html) to make type comparisons using [is_same_v](https://en.cppreference.com/w/cpp/types/is_same.html) simpler.

I decided to use constexpr to compute the result of type comparisons during compile time, as the type of the variant is defined at compile time with the template, so that the compiler could avoid compiling the unused if branch, saving time and avoiding possible type mismatch errors.

While searching ways to compute the mean of a vector, I found [this page](https://riptutorial.com/cplusplus/example/19185/using-std--nth-element-to-find-the-median--or-other-quantiles-), which suggested to use the function nth_element() as it is more efficient than sorting the whole vector.
  
On a side note, I used some string methods and some methods from [iomanip](https://cplusplus.com/reference/iomanip/) to format the output in a better way.


### main data analysis
The trickiest part in this section was the inclusion of the third party library [GetPot](https://sourceforge.net/projects/getpot/files/). The expectation of a single header file complete with the library implementation was not met, but thanks to the clear readme file and the examples provided, I decided to simply copy the three files with the implementation (GetPot as the interface, getpot.cpp and getpot.hpp containing the declarations and definitions) into the main folder, to be inlcuded as they are into the main statistical analysis.

We also spent a lot of time doing data cleaning to use some particular datasets found on kaggle, but in the end we opted for a simpler dataset to work with.

## ANALYSIS RESULTS

The statistical analysis module on the iris dataset ([source](https://www.kaggle.com/datasets/himanshunakrani/iris-dataset)), a dataset that is well known for its simple properties.

The analysis focuses on recreating known results regarding this dataset, to prove the correctness of the developed toolbox.

After building the executables with cmake and make, from the main folder, where the dataset resides, we run:
```bash
../CMakeFiles/main/main -f iris.csv
```

Opening the generated report, the results seem to match the expectations for this dataset. For example, the three different classes are balanced, and petal length and width are highly correlated. Interestingly, petal length is the only variable where median and mean are significantly different (median: 4.35; mean: 3.759) and it's also the variable with the highest standard deviation: this could suggest the presence of several outliers.

