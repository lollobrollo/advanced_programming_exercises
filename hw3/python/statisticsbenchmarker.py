from benchmarker import Benchmarker
from statisticsanalyzer import StatisticsAnalyzer

class StatisticsBenchmarker(Benchmarker):
    """
    Class used to compare time performance between equivalent functions from scicpp toolkit and pure python.
    """
    def __init__(self, descriptor:str, analyzer:StatisticsAnalyzer):
        Benchmarker.__init__(self, descriptor)
        self._analyzer = analyzer

    @Benchmarker.record_time("mean")
    def mean(self, col:str):
        return self._analyzer.mean(col)

    @Benchmarker.record_time("median")
    def median(self, col:str):
        return self._analyzer.median(col)

    @Benchmarker.record_time("variance")
    def var(self, col:str):
        return self._analyzer.var(col)

    @Benchmarker.record_time("std_deviation")
    def std_dev(self, col:str):
        return self._analyzer.std_dev(col)

    @Benchmarker.record_time("correlation")
    def corr(self, col1, col2):
        return self._analyzer.corr(col1, col2)

    def perform_analysis(self, col:str, comparison_col:str = None):
        """ Run computations and store timings """
        self.mean(col)
        self.median(col)
        self.var(col)
        if comparison_col:
            self.corr(col, comparison_col)