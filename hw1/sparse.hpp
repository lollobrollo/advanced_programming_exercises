#pragma once

typedef unsigned int uint;

#include <vector>

class TransientMatrixElement; // forward declaration

class SparseMatrix {
  public:
    virtual double operator()(const uint row, const uint col) const = 0; // read only
    virtual TransientMatrixElement operator()(const uint row, const uint col) = 0; // write access (for non const objects)
    virtual void setValue(const uint row, const uint col, const double value) = 0;
    // virtual SparseMatrix& operator*(const SparseMatrix& vec) const = 0; // dot product
    virtual std::vector<double> operator*(const std::vector<double>& vec) const = 0; // dot product
    virtual uint get_nrows() const = 0;
    virtual uint get_ncols() const = 0;
    virtual uint get_nonzeros() const = 0;
};
