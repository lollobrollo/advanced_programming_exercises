import scicpp # shared library built with pybind11
import time
import os
import math
from statistics import StatisticsAnalyzer
from integration import IntegralEvaluator

# before running this script, go to the root of hw3 and execcute the following command:
# export PYTHONPATH=$PYTHONPATH:$(pwd)/CMakeFiles/src
# this makes the pybind mibrary (which should have been build previously with cmake) available for import


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
    # Create an instance of the analyer
    perf_analyzer = StatisticsAnalyzer(ds)

    # Perform comparisons and produce report
    perf_analyzer.perform_analysis("Value", "Value")

    print("\n--- Generating Report ---")
    report_file = "stat_report.txt"
    # calling a C++ method with Python list inputs
    perf_analyzer.generate_report(report_file, ["Value", "Category"])
    if os.path.exists("hw3_report.txt"):
        print(f"Report successfully generated: {report_file}")

    print("\n--- Integration Correctness ---")
    # Test all integration strategies on x^2 from 0 to 3 (Expected result: 3^3 / 3 = 9.0)
    func, a, b, n, expected = 'x^2', 0.0, 3.0, 100, 9.0
    evaluator = IntegralEvaluator()
    evaluator.compare_all(func, a, b, n, expected)


    # Cleanup
    # os.remove(csv_file)
    # os.remove(report_file)
