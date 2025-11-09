#pragma once

typedef unsigned int uint;

#include <vector>

template <typename T> class TransientMatrixElement; // forward declaration

template <typename T> class SparseMatrix {
  friend class TransientMatrixElement<T>;

  protected:
    virtual void setValue(const uint row, const uint col, const T value) = 0;

  public:
    virtual T operator()(const uint row, const uint col) const = 0; // read only
    virtual TransientMatrixElement<T> operator()(const uint row, const uint col) = 0; // write access (for non const objects)
    // virtual SparseMatrix& operator*(const SparseMatrix& vec) const = 0; // dot product
    virtual std::vector<T> operator*(const std::vector<T>& vec) const = 0; // dot product
    virtual SparseMatrix<T>& transpose() const = 0;
    virtual uint get_nrows() const = 0;
    virtual uint get_ncols() const = 0;
    virtual uint get_nonzeros() const = 0;

};
