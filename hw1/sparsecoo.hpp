#pragma once

#include "sparse.hpp"
#include <vector>
#include <ostream>

typedef unsigned int uint;

class SparseMatrixCOO: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<double> values;
    std::vector<uint> rows;
    std::vector<uint> cols;

  public:
    SparseMatrixCOO(const uint nrows, const uint ncols); // constructor
    SparseMatrixCOO(const SparseMatrixCOO& other); // copy constructor
    SparseMatrixCOO& operator=(const SparseMatrixCOO& other); // copy assignment
    ~SparseMatrixCOO(); // destructor
  
    const double& operator()(const uint row, const uint col) const override; // read only (for const objects)
    double& operator()(const uint row, const uint col) override; // write access (for non const objects)
    SparseMatrix operator*(const SparseMatrix& vec) const override; // dot product
    SparseMatrix operator*(const std::vector<double>& vec) const override; // dot product
    const uint get_nrows() const override;
    const uint get_ncols() const override;
    const uint get_nonzeros() const override;


    friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat);
};

