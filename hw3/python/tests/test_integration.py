import unittest
from scicpp_python.integration import IntegralEvaluator

class TestIntegration(unittest.TestCase):
    """ 
    Just some basic tests for integration of python and c++ code
    The actual testing for functionality is done in the c++ part
    """
    def test_midpoint(self):
        evaluator = IntegralEvaluator(strategy="MidpointRule")
        self.assertAlmostEqual(evaluator("x", 0, 1, 1), 0.5)

    def test_trapezoidal(self):
        evaluator = IntegralEvaluator(strategy="TrapezoidalRule")
        self.assertAlmostEqual(evaluator("-x", 0, 1, 2), -0.5)

    def test_simpsons(self):
        evaluator = IntegralEvaluator(strategy="SimpsonsRule")
        self.assertAlmostEqual(evaluator("-x", 0, 1, 3), -0.5)

    def test_gaussian(self):
        evaluator = IntegralEvaluator(strategy="GaussianQuadrature")
        self.assertAlmostEqual(evaluator("x^4", 0, 1, 12), 0.2)

if __name__ == '__main__':
    unittest.main()