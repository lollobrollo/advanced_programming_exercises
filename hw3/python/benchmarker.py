import time

class Benchmarker():
    """ Helper class to benchmark functions using decorators and report comparisons """
    def __init__(self, descriptor:str):
        self._times = {}
        self._descriptor = descriptor
    
    def record_time(label):
        """ Decorator factory to record C++ execution time and result. """
        def decorator(func):
            def wrapper(self, *args, **kwargs):
                start = time.perf_counter()
                result = func(self, *args, **kwargs)
                end = time.perf_counter()
                self._times[label] = (end - start, result) # store a tuple, order mirrors name
                return result
            return wrapper
        return decorator
    

    def report_comparison(self, other):
        """ Helper that prints speedup and correctness comparison for each specific metric """
        for label in self.results.keys():
            if label in other.results.keys():
                my_time, my_res = self.results[label]
                other_time, other_res = other.results[label]
                speedup = other_time / my_time if my_time > 0 else 0
                
                print(f"\nMetric: {label.upper()}")
                print(f"  {self.descriptor}: {my_res:12.6f} | Time: {my_time:.6f}s")
                print(f"  {other.descriptor}: {other_res:12.6f} | Time: {other_time:.6f}s")
                print(f"  Speedup: {speedup:.2f}x faster using C++")

    @property
    def results(self):
        """Access the raw timing data from comparisons"""
        return self._times
    
    @property
    def descriptor(self):
        """Get description for benchmarker, used in comparisons between benchmarks"""
        return self._descriptor

    def __repr__(self):
        return f"<StatisticsBenchmarker(dataset_size={self._ds.size()})>"