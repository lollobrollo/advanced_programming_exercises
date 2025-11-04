#pragma once

#include "sparse.hpp"
#include <vector>

class SparseMatrixCSR: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    vector<double> values;
    vector<uint> cols;
    vector<uint> row_idx;
}