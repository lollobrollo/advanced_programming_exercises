from . import scicpp
from .statisticsanalyzer import StatisticsAnalyzer

class CppStatistics(StatisticsAnalyzer):
    """ Wrapper of the statistical analyzer provided by scicpp """
    def __init__(self, ds:scicpp.Dataset):
        self._ds = ds
        self.analyzer = scicpp.StatisticalAnalyzer(ds)

    def mean(self, col:str):
        return self.analyzer.mean(col)
    def median(self, col:str):
        return self.analyzer.median(col)
    def var(self, col:str):
        return self.analyzer.variance(col)
    def std_dev(self, col:str):
        return self.analyzer.std_deviation(col)
    def corr(self, col1:str, col2:str):
        return self.analyzer.correlation(col1, col2)

    def generate_report(self, file_name:str, col_names:list):
        self.analyzer.generate_report(file_name, col_names)