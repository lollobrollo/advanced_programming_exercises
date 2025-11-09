#pragma once

#include "sparse.hpp"
#include "transientmatrixelement.hpp"
#include <vector>
#include <ostream>

typedef unsigned int uint;

template<typename T> class SparseMatrixCSR; // forward declaration

template <typename T> class SparseMatrixCOO: public SparseMatrix<T> {
  private:
    const uint nrows;
    const uint ncols;
    std::vector<T> values;
    std::vector<uint> rows;
    std::vector<uint> cols;
    
  protected:
    void setValue(const uint row, const uint col, const T value) override;

  public:
    SparseMatrixCOO(const uint nrows, const uint ncols); // constructor
    SparseMatrixCOO(const SparseMatrixCOO<T>& other); // copy constructor
    SparseMatrixCOO(const SparseMatrixCSR<T>& other); // copy constructor
    SparseMatrixCOO<T>& operator=(const SparseMatrixCOO<T>& other); // copy assignment
    ~SparseMatrixCOO(); // destructor
  
    T operator()(const uint row, const uint col) const override; // read only (for const objects)
    TransientMatrixElement<T> operator()(const uint row, const uint col) override; // write access (for non const objects)
    //SparseMatrixCOO& operator*(const SparseMatrix& vec) const override; // dot product
    std::vector<T> operator*(const std::vector<T>& vec) const override; // dot product
    SparseMatrix<T>& transpose() const override;
    uint get_nrows() const override;
    uint get_ncols() const override;
    uint get_nonzeros() const override;

    template<typename U> friend std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO<U>& mat);

    friend class SparseMatrixCSR<T>;
};

