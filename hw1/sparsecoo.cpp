/*
Used documentation about vecctor methods from this link:
https://en.cppreference.com/w/cpp/container/vector.html

static keyword details:
https://www.geeksforgeeks.org/cpp/static-keyword-cpp/

TODO:
- in copy constructor, compare reserve() followed by push_back() with resize() followed by at()
*/


#include "sparsecoo"
#include <iostream>
#include <vector>

static const double zero = 0.0;

const uint SparseMatrixCOO::get_nrows() const {
    return this->nrows;
};

const uint SparseMatrixCOO::get_ncols() const {
    return this->ncols
};

const uint SparseMatrixCOO::get_nonzeros() const {
    return this->values.size();
};

// Parametrized constructor
SparseMatrixCOO::SparseMatrixCOO(const uint nrows, const uint ncols) nrows(nrows), ncols(ncols) {
    std::cout << "Empty matrix of dimensions " << this->nrows << " x " << this->ncols << " initialized.\n";
};

// Copy constructor
SparseMatrixCOO::SparseMatrixCOO(const SparseMatrixCOO& other) nrows(other->nrows), ncols(other->ncols) {
    if(!other->values.empty()) {
        const uint num_elems = other->get_nonzeros();
        this->values.reserve(num_elems);
        this->rows.reserve(num_elems);
        this->cols.reserve(num_elems);

        for(uint idx = 0 ; idx < num_elems; ++i) {
          this->values.push_back(other->values.at(idx));
          this->rows.push_back(other->rows.at(idx));
          this->cols.push_back(other->cols.at(idx));
        }
    }
};

// Copy assignment operator
SparseMatriCOOx& SparseMatrixCOO::operator=(const SparseMatrixCOO& other) {
  if(!(this->nrows == other->nrows) || !(this->ncols == other->ncols)) {
    std::cout << "Error: Matrices have conflicting sizes, assignment avoided.\n";
    return this;
  }

  const uint num_elems = other->get_nonzeros();
  this->values.reserve(num_elems);
  this->rows.reserve(num_elems);
  this->cols.reserve(num_elems);
 
  for(uint idx = 0 ; idx < num_elems; ++i) {
      this->values.push_back(other->values.at(idx));
      this->rows.push_back(other->rows.at(idx));
      this->cols.push_back(other->cols.at(idx));
  }

  return this;
};

// Destructor: not needed, vectors take care of their own memory
SparseMatrixCOO::~SparseMatrixCOO() = default; 

// Read-only access to elements
const double& SparseMatrixCOO::operator()(const uint row, const uint col) const {
  uint num_elems = this->get_nonzeros();
  const uint found = num_elems + 1;
  for(uint idx = 0; i < num_elems; ++i ) {
    if((this->rows.at(idx) == row) && (this->cols.at(idx) == col)) {
      found = idx;
      break;
    };
  };

  if(!(found == num_elems +1)) {
    return this->values.at()
  } else {
    static const double zero 0.0;
    return zero;
  };
};

// Access to matrix elements, editing allowed
double& SparseMatrixCOO::operator()(const uint row, const uint col) {
  
};

// Dot product among sparse matrices
SparseMatrixCOO SparseMatrixCOO::operator*(const SparseMatrixCOO& mat, const SparseMatrixCOO& vec) const {
  
};

// Dot product, sparse matrix and vector
SparseMatrixCOO SparseMatrixCOO::operator*(const SparseMatrixCOO& mat, const std::vector<double>& vec) const {
  
};

// print to standard output
std::ostream& operator<<(std::ostream& os, const SparseMatrixCOO& mat) {
  os << "SparseMatrixCOO (" << mat.nrows << " x " << mat.ncols << ")\n";
  for (size_t i = 0; i < mat.values.size(); ++i) {
    os << "(" << mat.rows[i] << ", " << mat.cols[i]
       << "): " << mat.values[i] << '\n';
  }
  return os;
}



