#pragma once

#include "sparse.hpp"
#include <vector>

typedef unsigned int uint;

template <typename T> class SparseMatrix; // forward declaration

template <typename T> class TransientMatrixElement {
  private:
    const uint locX;
    const uint locY;
    SparseMatrix<T>& matrix;

  public:
    TransientMatrixElement(uint i, uint j, SparseMatrix<T>& matrix);

    // Assignment operator
    TransientMatrixElement& operator=(const T& value);

    // conversion to double for read access
    operator T() const;
};
