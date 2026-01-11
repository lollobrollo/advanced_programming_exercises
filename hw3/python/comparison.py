import scicpp # shared library built with pybind11
import os
import math
from statisticsbenchmarker import StatisticsBenchmarker
from pystatistics import PyStatistics, SciPyStatistics
from cppstatistics import CppStatistics
from integration import IntegralEvaluator

# dummy dataset for testing
def create_test_csv(filename, rows=1000000):
    print(f"Creating a test dataset with {rows} rows...")
    with open(filename, "w") as f:
        f.write("ID,Value,Category\n")
        for i in range(rows):
            # Write a number and a category
            f.write(f"{i},{math.sin(i)},{'A' if i%2==0 else 'B'}\n")

csv_file = "test_data.csv"
if not os.path.exists(csv_file):
    create_test_csv(csv_file)


if __name__ == "__main__":
    # Load Dataset using C++
    ds = scicpp.Dataset(csv_file)

    # Create an instance of the analyzer (Python basic variant)
    py_analyzer = PyStatistics(ds)
    py_analyzer._get_column("Value") # Pre-extract values from dataset to make comparison fair
    py_perf_bench = StatisticsBenchmarker("Python", analyzer = py_analyzer)
    py_perf_bench.perform_analysis(col="Value", comparison_col="Value")

    # Create an instance of the analyzer (Python variant with numpy)
    scipy_analyzer = SciPyStatistics(ds)
    scipy_analyzer._get_column("Value") # Pre-extract values from dataset to make comparison fair
    scipy_perf_bench = StatisticsBenchmarker("Scientific Python", analyzer = scipy_analyzer)
    scipy_perf_bench.perform_analysis(col="Value", comparison_col="Value")


    # Create an instance of the analyzer (Cpp variant)
    cpp_perf_bench = StatisticsBenchmarker("C++", analyzer = CppStatistics(ds))
    cpp_perf_bench.perform_analysis(col="Value", comparison_col="Value")

    # Perform comparisons and produce report;
    # comparisons are made between our custom library and the python analyzers
    cpp_perf_bench.report_comparison(other=py_perf_bench)
    cpp_perf_bench.report_comparison(other=scipy_perf_bench)

    print("\n--- Generating Report ---")
    report_file = "stat_report.txt"
    # calling a C++ method with Python list inputs
    CppStatistics(ds).generate_report(report_file, ["Value", "Category"])
    if os.path.exists(report_file):
        print(f"Report successfully generated: {report_file}")

    print("\n--- Integration Correctness ---")
    # Test all integration strategies on x^2 from 0 to 3 (Expected result: 3^3 / 3 = 9.0)
    func, a, b, n, expected = 'x^2', 0.0, 3.0, 100, 9.0
    evaluator = IntegralEvaluator()
    evaluator.compare_all(func, a, b, n, expected)


    # Cleanup
    # os.remove(csv_file)
    # os.remove(report_file)
