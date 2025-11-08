// Testing grounds for the implemented classes

#include <iostream>
#include "sparsecoo.hpp"
#include "sparsecsr.hpp"

int main() {
    std::cout << "Hello, Sparse Matrix!\n";

    SparseMatrixCOO coo(5, 5);
    SparseMatrixCSR csr(5, 5);

    std::cout << "SparseMatrixCOO and SparseMatrixCSR created.\n";

    coo(1, 2) = 3.14;

    std::cout << "Writing to (1, 2) using TransientMatrixElement.\n";

    std::cout << "Element at (1, 2): " << coo(1, 2) << "\n";

    return 0;
}