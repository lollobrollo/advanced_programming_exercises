// Testing grounds for the implemented classes

#include <iostream>
#include <vector>
#include "sparsecoo.hpp"
#include "sparsecsr.hpp"

int main() {
    std::cout << "Hello, Sparse Matrix!\n";

    SparseMatrixCOO coo(3, 3);
    SparseMatrixCSR csr(5, 5);

    std::cout << "SparseMatrixCOO and SparseMatrixCSR created.\n\n";


    coo(1, 2) = 3.14;
    coo(2,1) = 4.25;
    std::cout << "Writing elements using TransientMatrixElement.\
                \n\nPrinting matrix to the standard output: " << coo << "\n";

    std::cout << "Element at (1, 2): " << coo(1, 2) << "\n\n";

    std::vector<double> vec(3, 0.0);
    vec.at(1) = 1.0;
    std::vector<double> res = coo * vec;
    std::cout << "Dot product, matrix * vector computed:\n[" << res.at(0) <<
    ", " << res.at(1) << ", " << res.at(2) << "]\n";

    csr(9,9) = 15.5; // Out of bounds assignment
    

    return 0;
}