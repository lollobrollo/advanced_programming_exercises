# make sure that scipi folder is available at PYTHONPATH
from . import scicpp

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
