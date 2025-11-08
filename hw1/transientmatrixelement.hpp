#pragma once

#include "sparse.hpp"
#include <vector>

typedef unsigned int uint;

class SparseMatrix;

class TransientMatrixElement {
  private:
    const uint locX;
    const uint locY;
    SparseMatrix& matrix;

  public:
    TransientMatrixElement(uint i, uint j, SparseMatrix& matrix);

    // Assignment operator
    TransientMatrixElement& operator=(const double& value);

    // conversion to double for read access
    operator double() const;
};
