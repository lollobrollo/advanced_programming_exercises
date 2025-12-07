// statistics/statistics.hpp
#pragma once

#include "dataset/dataset.hpp" // using cmake include paths, dataset header is included from the root of the project, so its folder needs to be explicitely written
#include <string>
#include <map>
#include <iostream>


class StatisticalAnalyzer {
// Class responsible for performing statistical analysis on a Dataset
public:
    StatisticalAnalyzer(const Dataset& dataset);

    // Numerical analysis
    double mean(const std::string& col_name) const;
    double median(const std::string& col_name) const;
    double variance(const std::string& col_name) const;
    double std_deviation(const std::string& col_name) const;

    // Pearson correlation between two numeric columns
    double correlation(const std::string& col1, const std::string& col2) const;

    // Returns a map asociating values to their frequency
    std::map<std::string, int> frequency_count(const std::string& col_name) const;

    // Summary written in a text file, considers columns passed in input
    void generate_report(const std::string& filename, const std::vector<std::string>& columns) const;

private:
    const Dataset& dataset;

    // Helper: extracts valid numeric values from a column; throws if the column is textual or empty
    std::vector<double> extract_numeric_data(const std::string& col_name) const;
};
