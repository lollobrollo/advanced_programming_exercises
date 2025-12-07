#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "GetPot"

#include "dataset/dataset.hpp"
#include "statistics/statistics.hpp"
#include "integration/integration.hpp"
#include "integration/midpoint_rule.hpp"
#include "integration/trapezoidal_rule.hpp"
#include "integration/simpsons_rule.hpp"
#include "integration/gaussian_quadrature.hpp"

using namespace SciCpp;

// used to highlight titles of sections
void print_header(const std::string& title) {
    std::cout << "\n" << std::string(50, '-') << "\n";
    std::cout << " " << title << "\n";
    std::cout << std::string(50, '-') << "\n";
}

int main(int argc, char** argv) {

    GetPot cl(argc, argv);
 
    // Check for integration flag
    std::string fn_expr = cl.follow("", 2, "-i", "--integrate");
    if (!fn_expr.empty()) {
        // Optional: allow user to select method and parameters
        std::string method = cl.follow("simpson", 2, "-m", "--method");
        double a = cl.follow(0.0, 2, "-a", "--start");
        double b = cl.follow(1.0, 2, "-b", "--end");
        int n_intervals = cl.follow(100, 2, "-n", "--intervals");

        std::unique_ptr<SciCpp::IntegrationStrategy> strategy;
        if (method == "midpoint") {
            strategy = std::make_unique<SciCpp::MidpointRule>();
        } else if (method == "trapezoidal") {
            strategy = std::make_unique<SciCpp::TrapezoidalRule>();
        } else if (method == "simpson") {
            strategy = std::make_unique<SciCpp::SimpsonsRule>();
        } else if (method == "gaussian") {
            strategy = std::make_unique<SciCpp::GaussianQuadrature>();
        } else {
            std::cerr << "Unknown integration method: " << method << "\n";
            return 1;
        }

        SciCpp::IntegralEvaluator evaluator(fn_expr, std::move(strategy));
        double result = evaluator(a, b, n_intervals);
        std::cout << "Integration of '" << fn_expr << "' from " << a << " to " << b << " using " << method << " rule (" << n_intervals << " intervals):\n";
        std::cout << "Result: " << result << std::endl;
        return 0;
    }

    // Default to "iris.csv"
    std::string filename = cl.follow("iris.csv", 2, "-f", "--file"); // format is (default, number of flags, flags to look for)
    if (!filename.empty()) {
        std::string report_file = cl.follow("iris_report.txt", 2, "-o", "--output");

        std::cout << ">> Loading Iris Dataset from: " << filename << "...\n";

        try {
            Dataset dataset(filename);
            std::cout << ">> Loaded " << dataset.size() << " samples.\n";

            StatisticalAnalyzer analyzer(dataset);

            //  Species distribution
            print_header("Class Balance (Species)");
            try {
                auto counts = analyzer.frequency_count("species");
                for (const auto& [name, count] : counts) {
                    std::cout << std::left << std::setw(20) << name << ": " << count << "\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }

            // Numerical statistics, sepals and petals
            std::vector<std::string> numeric_cols = {
                "sepal_length", "sepal_width", "petal_length", "petal_width"
            };

            print_header("Morphological Statistics");
            std::cout << std::left << std::setw(15) << "Feature" 
                    << std::setw(12) << "Mean" 
                    << std::setw(12) << "Median" 
                    << std::setw(12) << "StdDev" << "\n";
            std::cout << std::string(51, '.') << "\n";

            for (const auto& col : numeric_cols) {
                try {
                    double u = analyzer.mean(col);
                    double m = analyzer.median(col);
                    double s = analyzer.std_deviation(col);
                    
                    std::cout << std::left << std::setw(15) << col 
                            << std::setw(12) << std::setprecision(4) << u //setprecision works on the stream
                            << std::setw(12) << m 
                            << std::setw(12) << s << "\n";
                } catch (...) {
                    std::cout << std::left << std::setw(15) << col << " [Error]\n";
                }
            }

            // Correlation analysis -> petal length and petal width should be highly correlated
            print_header("Correlation Analysis");
            try {
                double r_petal = analyzer.correlation("petal_length", "petal_width");
                double r_sepal = analyzer.correlation("sepal_length", "sepal_width");

                std::cout << "Petal Length vs Width: " << r_petal <<"\n";
                std::cout << "Sepal Length vs Width: " << r_sepal << "\n";
            } catch (const std::exception& e) {
                std::cout << "Correlation error: " << e.what() << "\n";
            }

            // Generate and save report
            analyzer.generate_report(report_file, numeric_cols); // reuse numeric list
            std::cout << "\n>> Report saved to: " << report_file << "\n";

        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
            return 1;
        }
    }

    return 0;
}