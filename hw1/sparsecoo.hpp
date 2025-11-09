#pragma once

#include "sparse.hpp"
#include "transientmatrixelement.hpp"
#include <vector>
#include <ostream>

typedef unsigned int uint;

class SparseMatrixCSR; // forward declaration

class SparseMatrixCOO: public SparseMatrix {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<double> values;
    std::vector<uint> rows;
    std::vector<uint> cols;
    
  protected:
    void setValue(const uint row, const uint col, const double value) override;

  public:
    SparseMatrixCOO(const uint nrows, const uint ncols); // constructor
    SparseMatrixCOO(const SparseMatrixCOO& other); // copy constructor
    SparseMatrixCOO(const SparseMatrixCSR& other); // copy constructor
    SparseMatrixCOO& operator=(const SparseMatrixCOO& other); // copy assignment
    ~SparseMatrixCOO(); // destructor
  
    double operator()(const uint row, const uint col) const override; // read only (for const objects)
    TransientMatrixElement operator()(const uint row, const uint col) override; // write access (for non const objects)
    //SparseMatrixCOO& operator*(const SparseMatrix& vec) const override; // dot product
    std::vector<double> operator*(const std::vector<double>& vec) const override; // dot product
    uint get_nrows() const override;
    uint get_ncols() const override;
    uint get_nonzeros() const override;

    friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat);

    friend class SparseMatrixCSR;
};

