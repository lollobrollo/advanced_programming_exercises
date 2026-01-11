import unittest
import math
import scicpp
from cppstatistics import CppStatistics
from pystatistics import PyStatistics

# dummy dataset for testing
def create_test_csv(filename, rows=9):
    print(f"Creating a test dataset with {rows} rows...")
    with open(filename, "w") as f:
        f.write("ID,Value,Category\n")
        for i in range(rows):
            # Write a number and a category
            f.write(f"{i},{i+1},{'A' if i%2==0 else 'B'}\n")

csv_file = "test_data.csv"
create_test_csv(csv_file)

class TestStatistics(unittest.TestCase):
    """ 
    Just some basic tests for integration of python and c++ code
    The actual testing for functionality is done in the c++ part
    """
    def test_read_column(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertEqual(
            analyzer._get_column("Value"),
            ([1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0], 9))

    def test_pymean(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertAlmostEqual(
            analyzer.mean("Value"),
            5.0)


    def test_cppmean(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = CppStatistics(ds)
        self.assertAlmostEqual(
            analyzer.mean("Value"),
            5.0)

    def test_pymedian(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertAlmostEqual(
            analyzer.median("Value"),
            5.0)


    def test_cppmedian(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = CppStatistics(ds)
        self.assertAlmostEqual(
            analyzer.median("Value"),
            5.0)

    def test_pyvariance(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertAlmostEqual(
            analyzer.var("Value"),
            7.5)


    def test_cppvariance(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = CppStatistics(ds)
        self.assertAlmostEqual(
            analyzer.var("Value"),
            7.5)

    def test_pystd_dev(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertAlmostEqual(
            analyzer.std_dev("Value"),
            math.sqrt(7.5))


    def test_cppstd_dev(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = CppStatistics(ds)
        self.assertAlmostEqual(
            analyzer.std_dev("Value"),
            math.sqrt(7.5))

    def test_pycorr(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = PyStatistics(ds)
        self.assertAlmostEqual(
            analyzer.corr("Value", "Value"),
            1.0)


    def test_cppcorr(self):
        ds = scicpp.Dataset(csv_file)
        analyzer = CppStatistics(ds)
        self.assertAlmostEqual(
            analyzer.corr("Value", "Value"),
            1.0)

if __name__ == '__main__':
    unittest.main()