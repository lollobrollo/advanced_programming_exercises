import scicpp # shared library built with pybind11
import time
import os
import math

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



class StatisticalPerformanceAnalyzer():
    """
    Class usedd to compare time performance between equivalent functions from scipi toolkit and pure python
    """
    def __init__(self, ds:scicpp.Dataset):
        self._ds = ds
        self.analyzer = scicpp.StatisticalAnalyzer(ds)
        self._cpp_time_res = {}
        self._py_time_res = {}

    def record_time_res(label):
        """ Decorator factory to record C++ execution time and result. """
        def decorator(func):
            def wrapper(self, *args, **kwargs):
                start = time.perf_counter()
                result = func(self, *args, **kwargs)
                end = time.perf_counter()
                self._cpp_time_res[label] = (end - start, result) # store a tuple, order mirrors name
                return result
            return wrapper
        return decorator

    @record_time_res("mean")
    def _cpp_mean(self, col:str):
        return self.analyzer.mean(col)

    @record_time_res("median")
    def _cpp_median(self, col:str):
        return self.analyzer.median(col)

    @record_time_res("variance")
    def _cpp_var(self, col:str):
        return self.analyzer.variance(col)

    @record_time_res("std_dev")
    def _cpp_std(self, col:str):
        return self.analyzer.std_deviation(col)

    @record_time_res("correlation")
    def _cpp_corr(self, col1, col2):
        return self.analyzer.correlation(col1, col2)

    def _report_comparison(self, label):
        """ Helper that prints speedup and correctness comparison for a specific metric """
        cpp_time, cpp_res = self._cpp_time_res[label]
        py_time, py_res = self._py_time_res[label]
        speedup = py_time / cpp_time if cpp_time > 0 else 0
        
        print(f"\nMetric: {label.upper()}")
        print(f"  C++:    {cpp_res:12.6f} | Time: {cpp_time:.6f}s")
        print(f"  Python: {py_res:12.6f} | Time: {py_time:.6f}s")
        print(f"  Speedup: {speedup:.2f}x faster using C++")


    def perform_analysis(self, column:str, comparison_col:str = None):
        """ Performs all the comparisons required and shows the results on terminal """
        print("\n--- Performance Comparison Analysis ---") 
        # Data extraction and cleaning for python
        raw_data = self._ds.get_column(column)
        data = [v for v in raw_data if v is not None]
        n = len(data)

        # MEAN
        self._cpp_mean(column)
        t_start = time.perf_counter()
        res = sum(data) / n
        t_end = time.perf_counter()
        self._py_time_res["mean"] = (t_end - t_start, res) # store a tuple, order mirrors name
        self._report_comparison("mean")

        # MEDIAN
        self._cpp_median(column)
        t_start = time.perf_counter()
        sorted_data = sorted(data)
        if n % 2 == 1:
            py_res = sorted_data[n // 2]
        else:
            py_res = (sorted_data[n // 2 - 1] + sorted_data[n // 2]) / 2.0
        t_end = time.perf_counter()
        self._py_time_res["median"] = (t_end - t_start, py_res)
        self._report_comparison("median")

        # VARIANCE
        self._cpp_var(column)
        t_start = time.perf_counter()
        mu = sum(data) / n
        py_res = sum((x - mu)**2 for x in data) / (n - 1)
        t_end = time.perf_counter()
        self._py_time_res["variance"] = (t_end - t_start, py_res)
        self._report_comparison("variance")

        # CORRELATION (if second column is provided)
        if comparison_col:
            self._cpp_corr(column, comparison_col)
            raw_data2 = self._ds.get_column(comparison_col)
            data2 = [v for v in raw_data2 if v is not None]
            
            t_start = time.perf_counter()
            mu1, mu2 = sum(data)/n, sum(data2)/n
            num = sum((x - mu1) * (y - mu2) for x, y in zip(data, data2))
            den = math.sqrt(sum((x - mu1)**2 for x in data) * sum((y - mu2)**2 for y in data2))
            py_res = num / den if den != 0 else 0
            t_end = time.perf_counter()
            self._py_time_res["correlation"] = (t_end - t_start, py_res)
            self._report_comparison("correlation")

    @property
    def results(self):
        """Access the raw timing data, just in case"""
        return {"cpp": self._cpp_time_res, "python": self._py_time_res}

    def __repr__(self):
        return f"<StatisticalPerformanceAnalyzer(dataset_size={self._ds.size()})>"
    

if __name__ == "__main__":
    # Load Dataset using C++
    ds = scicpp.Dataset(csv_file)
    # Create an instance of the analyer
    perf_analyzer = StatisticalPerformanceAnalyzer(ds)

    # Perform comparisons and produce report
    perf_analyzer.perform_analysis("Value", "Value")


    print("\n--- Integration Correctness ---")

    # pass a C++ Simpson's Rule object into the C++ Evaluator from Python
    simpson = scicpp.SimpsonsRule()
    evaluator = scicpp.IntegralEvaluator("x^2", simpson)

    # Test integration of x^2 from 0 to 3 (Expected result: 3^3 / 3 = 9.0)
    a, b = 0.0, 3.0
    n = 100
    result = evaluator(a, b, n)

    print(f"Integrating 'x^2' from {a} to {b} with {n} intervals...")
    print(f"C++ Result: {result:.6f}")
    print(f"Expected: 9.000000")
    print(f"Error: {abs(9.0 - result):.2e}")



    print("\n--- Generating Report ---")
    report_file = "stat_report.txt"
    # calling a complex C++ method with Python list inputs
    perf_analyzer.analyzer.generate_report(report_file, ["Value", "Category"])
    if os.path.exists("hw3_report.txt"):
        print(f"Report successfully generated: {report_file}")

    # Cleanup
    # os.remove(csv_file)
    # os.remove(report_file)
