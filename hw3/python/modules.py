# make sure that scipi folder is available at PYTHONPATH
import scicpp
import math
import time

class StatisticsAnalyzer():
    """
    Class initially used to compare time performance between equivalent functions from scipi toolkit and pure python.
    It can also be used as a wrapper of the statistical analyzer provided by scicpp, while also providing some more functionalities.
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
    def cpp_mean(self, col:str):
        return self.analyzer.mean(col)

    @record_time_res("median")
    def cpp_median(self, col:str):
        return self.analyzer.median(col)

    @record_time_res("variance")
    def cpp_var(self, col:str):
        return self.analyzer.variance(col)

    @record_time_res("std_dev")
    def cpp_std(self, col:str):
        return self.analyzer.std_deviation(col)

    @record_time_res("correlation")
    def cpp_corr(self, col1, col2):
        return self.analyzer.correlation(col1, col2)

    def generate_report(self, file_name:str, col_names:list):
        self.analyzer.generate_report(file_name, col_names)

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
        self.cpp_mean(column)
        t_start = time.perf_counter()
        res = sum(data) / n
        t_end = time.perf_counter()
        self._py_time_res["mean"] = (t_end - t_start, res) # store a tuple, order mirrors name
        self._report_comparison("mean")

        # MEDIAN
        self.cpp_median(column)
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
        self.cpp_var(column)
        t_start = time.perf_counter()
        mu = sum(data) / n
        py_res = sum((x - mu)**2 for x in data) / (n - 1)
        t_end = time.perf_counter()
        self._py_time_res["variance"] = (t_end - t_start, py_res)
        self._report_comparison("variance")

        # CORRELATION (if second column is provided)
        if comparison_col:
            self.cpp_corr(column, comparison_col)
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
        """Access the raw timing data from comparisons, just in case"""
        return {"cpp": self._cpp_time_res, "python": self._py_time_res}

    def __repr__(self):
        return f"<StatisticsAnalyzer(dataset_size={self._ds.size()})>"
    
class IntegralEvaluator():
    """
    Class that wraps the integration module.
    Allows to change integration strategies and memorizzes last parameters used, to allow for faster tests
    """
    def __init__(self, strategy:str = "MidpointRule"):
        self._strategies = {
            "MidpointRule": scicpp.MidpointRule(),
            "TrapezoidalRule": scicpp.TrapezoidalRule(),
            "SimpsonsRule": scicpp.SimpsonsRule(),
            "GaussianQuadrature": scicpp.GaussianQuadrature()
        }
        self.current_strategy = self._strategies.get(strategy)

        self._latest_params = {
            "func":"0",
            "a": 1.0,
            "b": 2.0,
            "n": 3
        }

    def select_strategy(self, strat_name):
        if strat_name in self.available_strategies.keys():
            self.current_strategy = self._strategies[strat_name]
        else:
            print(f"Error: {strat_name} is not an available integration strategy.\nPlease choose from:\n{self._strategies.keys()}")

    def __call__(self, func: str=None, a: float=None, b: float=None, n: int=None):
        """ This function 'rememners' latest used params, and uses them if none is passed """
        if func is not None: self._latest_params["func"] = func
        if a is not None: self._latest_params["a"] = a
        if b is not None: self._latest_params["b"] = b
        if n is not None: self._latest_params["n"] = n

        evaluator = scicpp.IntegralEvaluator(
            self._latest_params["func"], 
            self.current_strategy
        )
        
        return evaluator(
            self._latest_params["a"], 
            self._latest_params["b"], 
            self._latest_params["n"]
        )

    def compare_all(self, func: str, a: float, b: float, n: int, expected: float):
        """Runs the integral with every strategy and prints an accuracy table."""
        print(f"Integrating {func} from {a} to {b} with {n} intervals...\n")
        for name, strat in self._strategies.items():
            print(f"Utilizing strategy {name}:")
            evaluator = scicpp.IntegralEvaluator(func, strat)
            result = evaluator(a, b, n)
            print(f"C++ Result: {result:.6f}")
            print(f"Expected: {expected:.6f}")
            print(f"Error: {abs(9.0 - result):.2e}\n")
