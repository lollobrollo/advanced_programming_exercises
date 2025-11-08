/*
Used documentation about vecctor methods from this link:
https://en.cppreference.com/w/cpp/container/vector.html

static keyword details:
https://www.geeksforgeeks.org/cpp/static-keyword-cpp/

TODO:
- in copy constructor, compare reserve() followed by push_back() with resize() followed by at()
*/


#include "sparsecoo.hpp"
#include "transientmatrixelement.hpp"
#include <iostream>
#include <ostream>
#include <vector>

typedef unsigned int uint;

uint SparseMatrixCOO::get_nrows() const {
    return this->nrows;
};

uint SparseMatrixCOO::get_ncols() const {
    return this->ncols;
};

uint SparseMatrixCOO::get_nonzeros() const {
    return this->values.size();
};

// Parametrized constructor
SparseMatrixCOO::SparseMatrixCOO(const uint nrows, const uint ncols) : nrows(nrows), ncols(ncols) {
    std::cout << "Empty matrix of dimensions " << this->nrows << " x " << this->ncols << " initialized.\n";
};

// Copy constructor
SparseMatrixCOO::SparseMatrixCOO(const SparseMatrixCOO& other) : nrows(other.nrows), ncols(other.ncols) {
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

// Copy assignment operator
SparseMatrixCOO& SparseMatrixCOO::operator=(const SparseMatrixCOO& other) {
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
SparseMatrixCOO::~SparseMatrixCOO() = default; 

// Read-only access to elements
double SparseMatrixCOO::operator()(const uint row, const uint col) const {
  uint num_elems = this->get_nonzeros();
  uint found = num_elems + 1;
  for(uint idx = 0; idx < num_elems; ++idx ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };

  return (found < num_elems +1) ? this->values.at(found) : 0.0;
};

// Access to matrix elements, editing allowed
TransientMatrixElement SparseMatrixCOO::operator()(const uint row, const uint col) {
  return TransientMatrixElement(row, col, *this);
};

void SparseMatrixCOO::setValue(const uint row, const uint col, const double value) {
  uint num_elems = this->get_nonzeros();
  uint found = num_elems + 1;
  for(uint idx = 0; idx < num_elems; ++idx ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };

  if(found < num_elems +1) {
    if(value != 0.0) {
      this->values.at(found) = value;
    } else {
      // Remove the element
      this->values.erase(this->values.begin() + found);
      this->rows.erase(this->rows.begin() + found);
      this->cols.erase(this->cols.begin() + found);
    };
  } else {
    if(value != 0.0) {
      this->values.push_back(value);
      this->rows.push_back(row);
      this->cols.push_back(col);
    };
  };
};

// Dot product among sparse matrices
SparseMatrixCOO& SparseMatrixCOO::operator*(const SparseMatrix& vec) const {
  throw std::logic_error("Error: Dot product operator not yet implemented.\n");
};

// Dot product, sparse matrix and vector
SparseMatrixCOO& SparseMatrixCOO::operator*(const std::vector<double>& vec) const {
  throw std::logic_error("Error: Dot product operator not yet implemented.\n");
};

// print to standard output
std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat) {
  os << "SparseMatrixCOO (" << mat.nrows << " x " << mat.ncols << ")\n";
  for (size_t idx = 0; idx < mat.values.size(); ++idx) {
    os << "(" << mat.rows.at(idx) << ", " << mat.cols.at(idx)
       << "): " << mat.values.at(idx) << '\n';
  };
  return os;
};



