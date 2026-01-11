import scicpp
import math
import numpy as np
from scipy import stats
from statisticsanalyzer import StatisticsAnalyzer

class PyStatistics(StatisticsAnalyzer):
    """ Equivalent implementation of scicpp in python """
    def __init__(self, ds:scicpp.Dataset):
        self._ds = ds
        self.cols = {}

    def _get_column(self, col:str):
        if not col in self.cols:
            raw_data = self._ds.get_column(col)
            self.cols[col] = [v for v in raw_data if v is not None]
        return (self.cols[col], len(self.cols[col]))

    def mean(self, col:str):
        data, n = self._get_column(col)

        return sum(data) / n

    def median(self, col:str):
        data, n = self._get_column(col)

        sorted_data = sorted(data)
        return (sorted_data[n // 2] 
            if n % 2 == 1 
            else (sorted_data[n // 2 - 1] + sorted_data[n // 2]) / 2.0
        )

    def var(self, col:str):
        data, n = self._get_column(col)

        mu = sum(data) / n
        return sum((x - mu)**2 for x in data) / (n - 1)

    def std_dev(self, col:str):
        return math.sqrt(self.var(col))

    def corr(self, col1:str, col2:str):
        data, n = self._get_column(col1)
        data2, _ = self._get_column(col2)

        mu1, mu2 = sum(data)/n, sum(data2)/n
        num = sum((x - mu1) * (y - mu2) for x, y in zip(data, data2))
        den = math.sqrt(sum((x - mu1)**2 for x in data) * sum((y - mu2)**2 for y in data2))
        return num / den if den != 0 else 0


class SciPyStatistics(StatisticsAnalyzer):
    """ Equivalent implementation of scicpp in python using NumPy and SciPy """
    def __init__(self, ds:scicpp.Dataset):
        self._ds = ds
        self.cols = {}

    def _get_column(self, col: str):
        if col not in self.cols:
            raw_data = self._ds.get_column(col)
            # Filter None and convert to a numpy array for SciPy compatibility
            filtered_data = [v for v in raw_data if v is not None]
            self.cols[col] = np.array(filtered_data)
        return self.cols[col]

    def mean(self, col: str):
        data = self._get_column(col)
        return np.mean(data)

    def median(self, col: str):
        data = self._get_column(col)
        return np.median(data)

    def var(self, col: str):
        data = self._get_column(col)
        return np.var(data, ddof=1) # ddof=1 -> division by (n-1)

    def std_dev(self, col: str):
        data = self._get_column(col)
        return np.std(data, ddof=1) #same as above

    def corr(self, col1: str, col2: str):
        data1 = self._get_column(col1)
        data2 = self._get_column(col2)
        
        correlation, _ = stats.pearsonr(data1, data2) # returns (correlation_coefficient, p_value)
        return correlation