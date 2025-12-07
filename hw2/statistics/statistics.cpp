/*
NOTES:
- visitor for dataset entries
  https://www.cppstories.com/2018/06/variant/
  https://www.cppstories.com/2018/09/visit-variants/

- decltype explanation (pretty straightforward)
  https://stackoverflow.com/questions/18815221/what-is-decltype-and-how-is-it-used

- decay_t is an helper type of std::decay, it returns the "base" type of an object
  https://en.cppreference.com/w/cpp/types/decay.html 
  
- is_same_v is an alias just like decay_t, and returns true if the two types are the same
  https://en.cppreference.com/w/cpp/types/is_same.html

- constexpr computes the result of the if statement at compile time, as the type of the variant is defined at compile time trough template,
  so that the compiler doesn't even try to compile the "wrong" branch, which could create a type mismatch error

- to compute the mean of a numerical vector, using nth_element() seems to be more efficient
  https://riptutorial.com/cplusplus/example/19185/using-std--nth-element-to-find-the-median--or-other-quantiles-

- In the report, when turning double values to strings, some string methods are used to format them in a more standard way (remove trailing zeros) 
  https://cplusplus.com/reference/string/string/
  */

// statistics/statistics.cpp
#include "statistics/statistics.hpp"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <iomanip> // used to prettify the output on file
#include <iostream>


StatisticalAnalyzer::StatisticalAnalyzer(const Dataset& dataset): dataset(dataset) {};


std::vector<double> StatisticalAnalyzer::extract_numeric_data(const std::string& col_name) const {
    const auto& column = this->dataset(col_name); // this is a variant containing a vector of optional elements, which can be either double or string
    // Vector of values that will be returned; space for them is reserved inside the visitor, when we have access to the size of the source data vector
    std::vector<double> clean_data;
    
    // Use a visitor to apply a (lambda) funciton on the elements contained in the variant column; in this case it is only applied once
    std::visit([&](const auto& vec) { // [&] is the default capture by reference: the lambda function captures all surrounding variables, probably not necessary but cool
        clean_data.reserve(vec.size()); // reserve space fro the output vector
        using VecType = std::decay_t<decltype(vec)>; // decay_t gets the "base" type of the item, to make a clean comparison later

        if constexpr (std::is_same_v<VecType, std::vector<std::optional<double>>>) { // consider the vector only if it's a number (double), not a string
            for (const auto& opt : vec) { // check for NA values
                if (opt.has_value()) {
                    clean_data.push_back(opt.value());
                }
            }
            if (clean_data.empty()) {
                throw std::runtime_error("Column has no valid numeric data: " + col_name);
            }
        } else {
            // we are considering a string column
            throw std::invalid_argument("Cannot perform numeric stats on string column: " + col_name);
        }
    }, column);
    
    return clean_data;
}


double StatisticalAnalyzer::mean(const std::string& col_name) const {
    auto data = this->extract_numeric_data(col_name);
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / static_cast<double>(data.size());
}


double StatisticalAnalyzer::median(const std::string& col_name) const {
    auto data = this->extract_numeric_data(col_name);
    size_t n = data.size();
    
    // Partial sort is more efficient than full sort for median, as reported in the notes on top
    size_t mid = n / 2; // a/b with a,b integers already truncates the eventual values after the dot
    std::nth_element(data.begin(), data.begin() + mid, data.end());
    double median_val = data[mid];

    if (n % 2 == 0) { // if vector has even number of items, we get the second "middle" one and average the two
        std::nth_element(data.begin(), data.begin() + mid - 1, data.end());
        median_val = (data[mid - 1] + median_val) / 2.0;
    }
    return median_val;
}


double StatisticalAnalyzer::variance(const std::string& col_name) const {
    auto data = this->extract_numeric_data(col_name);
    if (data.size() < 2) return 0.0;

    double sample_mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    
    double sum_sq_diff = std::accumulate(data.begin(), data.end(), 0.0, 
        [sample_mean](double acc, double val) { return acc + (val - sample_mean) * (val - sample_mean); });

    return sum_sq_diff / static_cast<double>(data.size() - 1); // sample variance
}


double StatisticalAnalyzer::std_deviation(const std::string& col_name) const {
    return std::sqrt(this->variance(col_name));
}   

    
double StatisticalAnalyzer::correlation(const std::string& col1, const std::string& col2) const {
    const auto& col1_vec = this->dataset(col1);
    const auto& col2_vec = this->dataset(col2);

    double correlation = 0.0;

    // Visit both variants simultaneously to keep corresponding pairs coupled
    std::visit([&](const auto& v1, const auto& v2) {
        using T1 = std::decay_t<decltype(v1)>; // check types just like in extract_numeric_data()
        using T2 = std::decay_t<decltype(v2)>;
        constexpr bool v1_double = std::is_same_v<T1, std::vector<std::optional<double>>>;
        constexpr bool v2_double = std::is_same_v<T2, std::vector<std::optional<double>>>;

            if constexpr (!v1_double || !v2_double) {
                throw std::invalid_argument("Columns must be numeric for correlation.");
            } else {
                if (v1.size() != v2.size()) {
                    throw std::invalid_argument("Columns must be of same length.");
                }

                // extract numerical values for later computations
                std::vector<double> x, y;
                x.reserve(v1.size());
                y.reserve(v1.size());
                for (size_t i = 0; i < v1.size(); ++i) {
                    if (v1[i].has_value() && v2[i].has_value()) {
                        x.push_back(v1[i].value());
                        y.push_back(v2[i].value());
                    }
                }

                size_t n = x.size();
                if (n == 0) {
                    throw std::runtime_error("One or more columns have no valid numeric data.");
                } else if (n == 1) {
                    // return with correlation == 0
                    correlation = 0.0;
                } else { // n >= 2
                    // Moving on to computing correlation, if vectors contain more than one data point
                    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
                    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
                    double mean_x = sum_x / n;
                    double mean_y = sum_y / n;

                    double num = 0.0;
                    double den_x = 0.0;
                    double den_y = 0.0;

                    for (size_t i = 0; i < n; ++i) {
                        double dx = x[i] - mean_x;
                        double dy = y[i] - mean_y;
                        num += dx * dy;
                        den_x += dx * dx;
                        den_y += dy * dy;
                    }

                    if (den_x == 0.0 || den_y == 0.0) {
                        correlation = 0.0;
                    } else {
                        correlation = num / std::sqrt(den_x * den_y);
                    };
                };
            };
        }, col1_vec, col2_vec);
    return correlation;
}


std::map<std::string, int> StatisticalAnalyzer::frequency_count(const std::string& col_name) const {
    std::map<std::string, int> counts; // this will store the counts this function will return
    const auto& column = this->dataset(col_name);

    std::visit([&](const auto& vec) {
        using VecType = std::decay_t<decltype(vec)>;

        for (const auto& opt_val : vec) {
            if (!opt_val.has_value()) {
                counts["NA"]++;
                continue;
            }

            if constexpr (std::is_same_v<VecType, std::vector<std::optional<std::string>>>) { // Handle vector of strings
                counts[opt_val.value()]++;
            } else { // Handle double vector
                std::string key = std::to_string(opt_val.value());
                key.erase(key.find_last_not_of('0') + 1, std::string::npos); // Remove trailing zeros
                if (key.back() == '.') key.pop_back();
                counts[key]++;
            }
        }
    }, column);

    return counts;
}


void StatisticalAnalyzer::generate_report(const std::string& filename, const std::vector<std::string>& columns) const {
    std::ofstream out(filename); // this also creates the file if it does not exist
    if (!out.is_open()) throw std::runtime_error("Could not open output file.");

    out << "Statistical Report\n==================\n\n";

    for (const auto& column : columns) {
        out << "Column: " << column << "\n";
        out << "-------------------\n";

        try {
            // Determine if column is numeric by trying to access it
            const auto& col_variant = this->dataset(column);
            std::visit([&](const auto& vec) {
                using VecType = std::decay_t<decltype(vec)>;
                
                if constexpr (std::is_same_v<VecType, std::vector<std::optional<std::string>>>) { // vector of string
                    out << "Categorical Data - Top Frequencies:\n";
                    auto freqs = frequency_count(column);
                    // freqs is a map, i want to order its values so i turn it into a vector of pairs
                    using PairType = std::pair<std::string, int>;
                    std::vector<PairType> sorted_freqs(freqs.begin(), freqs.end());
                    //now i can simply sort by value using a lambda function
                    std::sort(sorted_freqs.begin(), sorted_freqs.end(), 
                        [](const PairType& a, const PairType& b) {return a.second > b.second; });

                    int limit = 5;  // only write to the report the top 5 items, considering frequency
                    for(auto const& [key, val] : sorted_freqs) {
                        out << "  " << key << ": " << val << "\n";
                        if(--limit == 0) break;
                    }
                } else { // vector of double
                    out << std::left << std::setw(20) << "Mean:" << mean(column) << "\n";
                    out << std::left << std::setw(20) << "Median:" << median(column) << "\n";
                    out << std::left << std::setw(20) << "Std Dev:" << std_deviation(column) << "\n";
                }
            }, col_variant);

        } catch (const std::exception& e) {
            out << "  [Analysis failed: " << e.what() << "]\n";
        };
        out << "\n";
    }
    std::cout << "Report generated at: " << filename << std::endl;
}
