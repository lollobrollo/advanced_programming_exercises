#pragma once

#include "sparse.hpp"
#include <vector>

class SparseMatrixCOO: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    vector<double> values;
    vector<uint> rows;
    vector<uint> cols;

  public:
    SparseMatrixCOO(const uint nrows, const uint ncols); // constructor
    SparseMatrixCOO(const SparseMatrix& other); // copy constructor
    SparseMatrixCOO& operator=(const SparseMatrix& other); // copy assignment
    ~SparseMatrixCOO(); // destructor

}

