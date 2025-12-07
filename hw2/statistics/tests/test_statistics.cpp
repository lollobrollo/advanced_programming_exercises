#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "dataset/dataset.hpp"
#include "statistics/statistics.hpp"


// creates a CSV with two correlated columns
static std::string make_numeric_csv() {
    std::string path = "test_stats_numeric.csv";
    std::ofstream out(path);
    out << "val1,val2\n";
    out << "1.0,2.0\n";
    out << "2.0,4.0\n";
    out << "3.0,6.0\n";
    out << "4.0,8.0\n";
    out << "5.0,10.0\n";
    out.close();
    return path;
}

// Creates a CSV with mixed types and NAs
static std::string make_mixed_csv() {
    std::string path = "test_stats_mixed.csv";
    std::ofstream out(path);
    out << "cat,with_na,constant\n";
    out << "A,10.0,5.0\n";
    out << "B,,5.0\n";
    out << "A,30.0,5.0\n";
    out << "C,,5.0\n";
    out << "A,50.0,5.0\n";
    out.close();
    return path;
}

// creates a CSV with an even number of rows for median testing
static std::string make_even_csv() {
    std::string path = "test_stats_even.csv";
    std::ofstream out(path);
    out << "vals\n";
    out << "10.0\n";
    out << "20.0\n";
    out << "30.0\n";
    out << "40.0\n";
    out.close();
    return path;
}

TEST(StatisticsTest, MeanCalculation) {
    auto path = make_numeric_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // 1+2+3+4+5 = 15 / 5 = 3.0
    EXPECT_DOUBLE_EQ(stats.mean("val1"), 3.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, MedianCalculationOdd) {
    auto path = make_numeric_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // 1, 2, 3, 4, 5
    EXPECT_DOUBLE_EQ(stats.median("val1"), 3.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, MedianCalculationEven) {
    auto path = make_even_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // 10, 20, 30, 40
    EXPECT_DOUBLE_EQ(stats.median("vals"), 25.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, VarianceAndStdDev) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // Constant column has 0 variance
    EXPECT_DOUBLE_EQ(stats.variance("constant"), 0.0);
    EXPECT_DOUBLE_EQ(stats.std_deviation("constant"), 0.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, VarianceNumeric) {
    auto path = make_numeric_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // 1, 2, 3, 4, 5 -> mean=3
    // Squared differences: 4, 1, 0, 1, 4 -> sum 10
    // Var = 10 / (5-1) = 2.5
    EXPECT_DOUBLE_EQ(stats.variance("val1"), 2.5);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, IgnoreNAValues) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // valid values: 10, 30, 50
    EXPECT_DOUBLE_EQ(stats.mean("with_na"), 30.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, FrequencyCountCategorical) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);
    
    // A, B, A, C, A
    auto counts = stats.frequency_count("cat");
    
    EXPECT_EQ(counts["A"], 3);
    EXPECT_EQ(counts["B"], 1);
    EXPECT_EQ(counts["C"], 1);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, FrequencyCountNumeric) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // 5.0 appears 5 times
    auto counts = stats.frequency_count("constant");
    EXPECT_EQ(counts["5"], 5);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, CorrelationPerfect) {
    auto path = make_numeric_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // val2 is exactly 2 * val1 -> correlation = 1.0
    EXPECT_DOUBLE_EQ(stats.correlation("val1", "val2"), 1.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, CorrelationZero) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // Correlation with a constant variable -> return 0
    EXPECT_DOUBLE_EQ(stats.correlation("with_na", "constant"), 0.0);

    std::filesystem::remove(path);
}

TEST(StatisticsTest, ErrorHandling) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    // mean on string column
    EXPECT_THROW(stats.mean("cat"), std::invalid_argument);

    // correlation on string column
    EXPECT_THROW(stats.correlation("cat", "constant"), std::invalid_argument);

    // Invalid column, dataset class throws an exception
    EXPECT_ANY_THROW(stats.mean("fblthp_the_lost"));

    std::filesystem::remove(path);
}

TEST(StatisticsTest, GenerateReport) {
    auto path = make_mixed_csv();
    Dataset ds(path);
    StatisticalAnalyzer stats(ds);

    std::string report_path = "test_report_temp.txt";
    std::vector<std::string> cols = {"cat", "with_na"};
    
    EXPECT_NO_THROW(stats.generate_report(report_path, cols));

    // Check if file exists
    std::ifstream f(report_path);
    EXPECT_TRUE(f.good());
    f.close();

    std::filesystem::remove(path);
    std::filesystem::remove(report_path);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}