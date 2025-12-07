
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
- GetPot, third party library for command line parsing, extracted in *main* folder. If not present, can be downloaded [here](https://getpot.sourceforge.net/)

To compile the project, simply run the following commands from the root of the project

```bash
cmake -S. -BCMakeFiles 
cmake --build CMakeFiles
```

To perform tests on the compiled modules, switch to the CMakeFiles folder and type:
```bash
ctest --output-on-failure
```

## IMPLEMENTATION NOTES

### dataset module
...

### integration module
...

### statistics module
...

### main data analysis
... (e.g. getpot inclusion)

## ANALYSIS RESULTS

The statistical analysis module on the kaggle dataset "Video Game Sales" ([source](https://www.kaggle.com/datasets/lamskdna/video-games-sales)) contains ...

The analysis focused on ...

To run the analysis, call the executable with this flag ...

