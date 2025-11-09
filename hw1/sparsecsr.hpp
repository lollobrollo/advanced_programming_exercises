#pragma once

#include "sparse.hpp"
#include "transientmatrixelement.hpp"
#include <vector>
#include <ostream>

typedef unsigned int uint;

template<typename T> class SparseMatrixCOO; // forward declaration

template<typename T> class SparseMatrixCSR: public SparseMatrix<T> {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<T> values;
    std::vector<uint> cols;
    std::vector<uint> row_idx;
    
  protected:
    void setValue(const uint row, const uint col, const T value) override;

  public:
    SparseMatrixCSR(const uint nrows, const uint ncols); // constructor
    SparseMatrixCSR(const SparseMatrixCSR<T>& other); // copy constructor
    SparseMatrixCSR(const SparseMatrixCOO<T>& other); // copy constructor
    SparseMatrixCSR<T>& operator=(const SparseMatrixCSR<T>& other); // copy assignment
    ~SparseMatrixCSR(); // destructor

    T operator()(const uint row, const uint col) const override; // read only (for const objects)
    TransientMatrixElement<T> operator()(const uint row, const uint col) override; // write access (for non const objects)
    //SparseMatrixCSR& operator*(const SparseMatrix& vec) const override; // dot product
    std::vector<T> operator*(const std::vector<T>& vec) const override; // dot product
    SparseMatrix<T>& transpose() const override;
    uint get_nrows() const override;
    uint get_ncols() const override;
    uint get_nonzeros() const override;

    template<typename U> friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCSR<U>& mat);

    friend class SparseMatrixCOO<T>;
};