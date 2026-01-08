import scicpp
import time
import os
import math

# dummy dataset for testing
def create_test_csv(filename, rows=1000000):
    print(f"Creating a test dataset with {rows} rows...")
    with open(filename, "w") as f:
        f.write("ID,Value,Category\n")
        for i in range(rows):
            # Write a number and a category
            f.write(f"{i},{math.sin(i)},{'A' if i%2==0 else 'B'}\n")

CSV_FILE = "test_data.csv"
if not os.path.exists(CSV_FILE):
    create_test_csv(CSV_FILE)


print("\n--- Performance Analysis ---")

# Load Dataset using C++
ds = scicpp.Dataset(CSV_FILE)
analyzer = scicpp.StatisticalAnalyzer(ds)

# Benchmark C++ Mean
start_cpp = time.perf_counter()
cpp_mean = analyzer.mean("Value")
end_cpp = time.perf_counter()
cpp_time = end_cpp - start_cpp

# Benchmark Pure Python Mean
column_data = ds.get_column("Value")
start_py = time.perf_counter()
clean_data = [val for val in column_data if val is not None]
py_mean = sum(clean_data) / len(clean_data)
end_py = time.perf_counter()
py_time = end_py - start_py

print(f"C++ Mean Result: {cpp_mean:.6f} | Time: {cpp_time:.6f}s")
print(f"Python Mean Result: {py_mean:.6f} | Time: {py_time:.6f}s")
print(f"Speedup: {py_time / cpp_time:.2f}x faster using C++ logic.")



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
# calling a complex C++ method with Python list inputs
analyzer.generate_report("hw3_report.txt", ["Value", "Category"])
if os.path.exists("hw3_report.txt"):
    print("Report successfully generated: hw3_report.txt")

# Cleanup
# os.remove(CSV_FILE)