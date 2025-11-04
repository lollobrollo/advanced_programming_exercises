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

#include <vector>

class SparseMatrix {
  public:
    virtual SparseMatrix(const uint nrows, const uint ncols) = 0; // constructor
    virtual SparseMatrix(const SparseMatrix& other) = 0; // copy constructor
    virtual SparseMatrix& operator=(const SparseMatrix& other) = 0; // copy assignment
    virtual ~SparseMatrix() = 0; // destructor
    
    virtual const double& operator()(const uint row, const uint col) const = 0; // read only (for const objects)
    virtual double& operator()(const uint row, const uint col) = 0; // write access (for non const objects)
    virtual SparseMatrix operator*(const SparseMatrix& mat, const SparseMatrix& vec) const = 0; // dot product
    virtual SparseMatrix operator*(const SparseMatrix& mat, const std::vector<double>& vec) const = 0; // dot product
    virtual std::ostream& operator<<(std::ostream& os, const SparseMatrix& mat) = 0; // custom output for pretty printing

    virtual const uint get_nrows() const = 0;
    virtual const uint get_ncols() const = 0;
    virtual const uint get_nonzeros() const = 0;
    
};
