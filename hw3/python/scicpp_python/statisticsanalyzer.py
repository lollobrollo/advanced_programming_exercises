class StatisticsAnalyzer():
    """ Generic statistics analyzer interface specification """
    def mean(self, col:str):
        raise NotImplementedError()
    def median(self, col:str):
        raise NotImplementedError()
    def var(self, col:str):
        raise NotImplementedError()
    def std_dev(self, col:str):
        raise NotImplementedError()
    def corr(self, col1:str, col2:str):
        raise NotImplementedError()