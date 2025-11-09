#pragma once

#include "sparse.hpp"
#include "transientmatrixelement.hpp"
#include <vector>
#include <ostream>

typedef unsigned int uint;

class SparseMatrixCSR: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<double> values;
    std::vector<uint> cols;
    std::vector<uint> row_idx;

  public:
    SparseMatrixCSR(const uint nrows, const uint ncols); // constructor
    SparseMatrixCSR(const SparseMatrixCSR& other); // copy constructor
    SparseMatrixCSR& operator=(const SparseMatrixCSR& other); // copy assignment
    ~SparseMatrixCSR(); // destructor

    double operator()(const uint row, const uint col) const override; // read only (for const objects)
    TransientMatrixElement operator()(const uint row, const uint col) override; // write access (for non const objects)
    void setValue(const uint row, const uint col, const double value) override;
    //SparseMatrixCSR& operator*(const SparseMatrix& vec) const override; // dot product
    std::vector<double> operator*(const std::vector<double>& vec) const override; // dot product
    uint get_nrows() const override;
    uint get_ncols() const override;
    uint get_nonzeros() const override;

    friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCSR& mat);
};