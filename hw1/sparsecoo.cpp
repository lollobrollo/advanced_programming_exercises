/*
Used documentation about vecctor methods from this link:
https://en.cppreference.com/w/cpp/container/vector.html

static keyword details:
https://www.geeksforgeeks.org/cpp/static-keyword-cpp/

TODO:
- in copy constructor, compare reserve() followed by push_back() with resize() followed by at()
*/


#include "sparsecoo.hpp"
#include "sparsecsr.hpp"
#include "transientmatrixelement.hpp"
#include <iostream>
#include <ostream>
#include <vector>

typedef unsigned int uint;

template <typename T> uint SparseMatrixCOO<T>::get_nrows() const {
    return this->nrows;
};

template <typename T> uint SparseMatrixCOO<T>::get_ncols() const {
    return this->ncols;
};

template <typename T> uint SparseMatrixCOO<T>::get_nonzeros() const {
    return this->values.size();
};

// Parametrized constructor
template <typename T> SparseMatrixCOO<T>::SparseMatrixCOO(const uint nrows, const uint ncols) : nrows(nrows), ncols(ncols) {
    std::cout << "Empty matrix of dimensions " << this->nrows << " x " << this->ncols << " initialized.\n";
};

// Copy constructor
template <typename T> SparseMatrixCOO<T>::SparseMatrixCOO(const SparseMatrixCOO<T>& other) : nrows(other.nrows), ncols(other.ncols) {
    if(!other.values.empty()) {
        const uint num_elems = other.get_nonzeros();
        this->values.reserve(num_elems);
        this->rows.reserve(num_elems);
        this->cols.reserve(num_elems);

        for(uint idx = 0 ; idx < num_elems; ++idx) {
          this->values.push_back(other.values.at(idx));
          this->rows.push_back(other.rows.at(idx));
          this->cols.push_back(other.cols.at(idx));
        }
    }
};

// Copy constructor from CSR
template <typename T> SparseMatrixCOO<T>::SparseMatrixCOO(const SparseMatrixCSR<T>& other) : nrows(other.get_nrows()), ncols(other.get_ncols()) {
    // Convert CSR to COO
    this->values.clear();
    this->rows.clear();
    this->cols.clear();
    for (uint row = 0; row < other.nrows; ++row) {
        uint row_start = other.row_idx.at(row);
        uint row_end = other.row_idx.at(row + 1);
        for (uint idx = row_start; idx < row_end; ++idx) {
            this->values.push_back(other.values.at(idx));
            this->rows.push_back(row);
            this->cols.push_back(other.cols.at(idx));
        }
    }
}

// Copy assignment operator
template <typename T> SparseMatrixCOO<T>& SparseMatrixCOO<T>::operator=(const SparseMatrixCOO<T>& other) {
  if(!(this->nrows == other.nrows) || !(this->ncols == other.ncols)) {
    std::cout << "Error: Matrices have conflicting sizes, assignment avoided.\n";
    return *this;
  };

  const uint num_elems = other.get_nonzeros();
  this->values.reserve(num_elems);
  this->rows.reserve(num_elems);
  this->cols.reserve(num_elems);
 
  for(uint idx = 0 ; idx < num_elems; ++idx) {
      this->values.push_back(other.values.at(idx));
      this->rows.push_back(other.rows.at(idx));
      this->cols.push_back(other.cols.at(idx));
  };

  return *this;
};

// Destructor: not needed, vectors take care of their own memory
template <typename T> SparseMatrixCOO<T>::~SparseMatrixCOO() = default; 

// Read-only access to elements
template <typename T> T SparseMatrixCOO<T>::operator()(const uint row, const uint col) const {
  if((row >= this->nrows) || (col >= this->ncols)) {
    std::cout << "Error: assigning value out of matrix bounds. Assignment avoided.\n";
    return 0.0;
  };
  uint num_elems = this->get_nonzeros();
  uint found = num_elems + 1;
  // Check wether element with coordinates (i,j) already exists
  for(uint idx = 0; idx < num_elems; ++idx ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };
  // If searched element was not initialized, return 0.0 by default
  return (found < num_elems +1) ? this->values.at(found) : 0.0;
};

// Access to matrix elements, editing allowed: managed by external class
template <typename T> TransientMatrixElement<T> SparseMatrixCOO<T>::operator()(const uint row, const uint col) {
  if((row >= this->nrows) || (col >= this->ncols)) {
    throw std::out_of_range("Matrix indices out of bounds");
  };
  return TransientMatrixElement(row, col, *this);
};

// Function used by external proxy to access to matrix data
template <typename T> void SparseMatrixCOO<T>::setValue(const uint row, const uint col, const T value) {
  uint num_elems = this->get_nonzeros();
  uint found = num_elems + 1;
  // Check for matching coordinates
  for(uint idx = 0; idx < num_elems; ++idx ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };

  if(found < num_elems +1) { // if element already exist in the matrix
    if(value != 0.0) { // if we are not assigning zero, overwrite the value
      this->values.at(found) = value;
    } else { // if we are assigning zero, remove the element from the matrix
      this->values.erase(this->values.begin() + found);
      this->rows.erase(this->rows.begin() + found);
      this->cols.erase(this->cols.begin() + found);
    };
  } else { // If no value was initialized at given indexes
    if(value != 0.0) { // If we are not assigning zero, save the value
      this->values.push_back(value);
      this->rows.push_back(row);
      this->cols.push_back(col);
    };
  };
};

// // Dot product among sparse matrices
// SparseMatrixCOO& SparseMatrixCOO::operator*(const SparseMatrix& other) const {
//   if(!(this->ncols == other.nrows)) {
//    std::cout << "Error: sizes of matrices do not match.\n";
//   return this;
//   };

//   SparseMatrixCOO result(this->nrows, other->ncols);
//   for(uint i = 0; i < this->values.size(); ++i) {
//     for(uint j = 0; j < other.values.size(); ++j) {
//       if(this->cols.at(i) == other.rows.at(j)) {
//         result(this->rows.at(i), other.cols.at(j)) += this->values.at(i) * other.values.at(i);
//       };
//     };
//   };
//   return result;
// };

// Dot product, sparse matrix and vector+
template <typename T> std::vector<T> SparseMatrixCOO<T>::operator*(const std::vector<T>& vec) const {
  if(!(this->nrows == vec.size())) {
    std::cout << "Error: sizes of matrix and vector do not match.\n";
    return vec;
  };

  std::vector<T> result(this->nrows, T{}); // skipped rows are defaulted to 0.0
  for(uint idx = 0; idx < this->values.size(); ++idx) {
    result.at(this->rows.at(idx)) += this->values.at(idx) * vec.at(idx); // Iteratively build result
  };
  return result;
};

// print to standard output
template <typename T> std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO<T>& mat) {
  os << "SparseMatrixCOO (" << mat.nrows << " x " << mat.ncols << ")\n";
  for (size_t idx = 0; idx < mat.values.size(); ++idx) {
    os << "(" << mat.rows.at(idx) << ", " << mat.cols.at(idx)
       << "): " << mat.values.at(idx) << '\n';
  };
  return os;
};

template <typename T> SparseMatrix<T>& SparseMatrixCOO<T>::transpose() const {
    SparseMatrixCOO<T> t(this->ncols, this->nrows); // Create a new object to be returned
    t.values.reserve(this->values.size());
    t.rows.reserve(this->rows.size());
    t.cols.reserve(this->cols.size());

    for (size_t i = 0; i < this->values.size(); ++i) {
      // Copy values as is, swap indexes of rows and columns
      t.values.push_back(this->values[i]);
      t.rows.push_back(this->cols[i]);
      t.cols.push_back(this->rows[i]);
    };
    return t;
};

// Explicit template instantiation
template class SparseMatrixCOO<double>;
template std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO<double>& mat);