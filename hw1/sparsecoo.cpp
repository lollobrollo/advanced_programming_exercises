/*
consulted sites:
https://en.cppreference.com/w/cpp/container/vector.html
https://www.geeksforgeeks.org/cpp/static-keyword-cpp/
https://en.cppreference.com/w/cpp/utility/tuple.html

TODO:
- in copy constructor, compare reserve() followed by push_back() with resize() followed by at()


- i expect that elements are ordered during insertion (row and column indexes-wise)
*/


#include <iostream>
#include <ostream>
#include <vector>
#include "sparsecoo.hpp"

typedef unsigned int uint;


const uint SparseMatrixCOO::get_nrows() const {
    return this->nrows;
};

const uint SparseMatrixCOO::get_ncols() const {
    return this->ncols;
};

const uint SparseMatrixCOO::get_nonzeros() const {
    return this->entries.size();
};

// Parametrized constructor
SparseMatrixCOO::SparseMatrixCOO(const uint nrows, const uint ncols) : nrows(nrows), ncols(ncols) {
    std::cout << "Empty matrix of dimensions " << this->nrows << " x " << this->ncols << " initialized.\n";
};

// Copy constructor
SparseMatrixCOO::SparseMatrixCOO(const SparseMatrixCOO& other) : nrows(other.nrows), ncols(other.ncols) {
  if(!other.entries.empty()) {
    this->entries.reserve(other.entries.size());
    for(const MyTuple& entry : other.entries) {
      this->entries.push_back(entry);
    };
  };
};

// Copy assignment operator
SparseMatrixCOO& SparseMatrixCOO::operator=(const SparseMatrixCOO& other) {
  if(!(this->nrows == other.nrows) || !(this->ncols == other.ncols)) {
    std::cout << "Error: Matrices have conflicting sizes, assignment avoided.\n";
    return *this;
  };

  if(this != other) {
  this->entries.clear();
  this->entries.reserve(other.entries.size());
  for(const MyTuple& entry : other.entries) { this->entries.push_back(entry); };
  };
  return *this;
};

// Destructor
SparseMatrixCOO::~SparseMatrixCOO() = default;

// Read-only access to elements
const double& SparseMatrixCOO::operator()(const uint row, const uint col) const {
  uint num_elems = this->get_nonzeros();
  uint found = num_elems + 1;
  for(uint idx = 0; idx < num_elems; ++idx ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };

  if(!(found == num_elems +1)) {
    return this->entries.at(found);
  } else {
    static const double zero = 0.0;
    return zero;
  };
};

// Access to matrix elements, editing allowed
double& SparseMatrixCOO::operator()(const uint row, const uint col) {
  
};

// Dot product among sparse matrices
SparseMatrixCOO SparseMatrixCOO::operator*(const SparseMatrixCOO& vec) const {

};

// Dot product, sparse matrix and vector
SparseMatrixCOO SparseMatrixCOO::operator*(const std::vector<double>& vec) const {
  
};

// print to standard output
std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat) {
  os << "SparseMatrixCOO (" << mat.nrows << " x " << mat.ncols << ")\n";
  for (size_t idx = 0; idx < mat.entries.size(); ++idx) {
    os << "(" << mat.rows.at(idx) << ", " << mat.cols.at(idx)
       << "): " << mat.entries.at(idx) << '\n';
  };
  return os;
};



