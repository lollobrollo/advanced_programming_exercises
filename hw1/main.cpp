// Testing grounds for the implemented classes

#include <iostream>
#include <vector>
#include "sparsecoo.hpp"
#include "sparsecsr.hpp"

int main() {
    std::cout << "Hello, Sparse Matrix!\n";

    SparseMatrixCOO coo(3, 3);
    SparseMatrixCSR csr(5, 5);

    std::cout << "SparseMatrixCOO and SparseMatrixCSR created.\n";

    
    coo(1, 2) = 3.14;
    std::cout << "Writing to (1, 2) using TransientMatrixElement.\n";
    
    std::cout << "Element at (1, 2): " << coo(1, 2) << "\n";
    
    std::vector<double> vec(3, 2);
    std::vector<double> res = coo * vec;
    std::cout << "Dot product, matrix * vector computed.\n";

    return 0;
}