#pragma once

#include "sparse.hpp"
#include <vector>
#include <ostream>
#include <tuple>

typedef unsigned int uint;

class SparseMatrixCOO: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<double> entries; // vector of MyTuple elements

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

    friend class MtTuple;
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat);
};

// Class used to save values associated with corresponding row and column coordinates
class MyTuple {
  private:
    std::tuple<uint, uint, double> mat_entry; // row, colum, value
    SparseMatrix& mat;

  public:
    MyTuple(uint row, uint col, double val, SparseMatrixCOO& mat);
    MyTuple(const MyTuple& other); // copy constructor (invoked by vector.push_back(MyTuole&))
    MyTuple& operator=(const MyTuple& other); // copy assignment 
};