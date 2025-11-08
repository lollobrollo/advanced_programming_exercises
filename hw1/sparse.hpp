/*
Ah yes, is this the best way to share thoughts? Of course it is, it's the comments in the working file

Random stuff:
- see wether int vals can be converted to uint

Proposals:
- overload dot product to make it work for vectors that are both std::vector<> and SparseMatrix
- templatize stuff
- 

*/


#pragma once

typedef unsigned int uint;

#include <vector>

class SparseMatrix {
  public:
    virtual const double& operator()(const uint row, const uint col) const = 0; // read only (for const objects)
    virtual double& operator()(const uint row, const uint col) = 0; // write access (for non const objects)
    virtual SparseMatrix& operator*(const SparseMatrix& vec) const = 0; // dot product
    virtual SparseMatrix& operator*(const std::vector<double>& vec) const = 0; // dot product
    virtual uint get_nrows() const = 0;
    virtual uint get_ncols() const = 0;
    virtual uint get_nonzeros() const = 0;
};
