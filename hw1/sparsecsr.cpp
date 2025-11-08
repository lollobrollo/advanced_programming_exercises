#include "sparsecsr.hpp"
#include <iostream>

uint SparseMatrixCSR::get_nrows() const {
    return this->nrows;
};

uint SparseMatrixCSR::get_ncols() const {
    return this->ncols;
};

uint SparseMatrixCSR::get_nonzeros() const {
    return this->values.size();
};

// Parametrized constructor
SparseMatrixCSR::SparseMatrixCSR(const uint nrows, const uint ncols) : nrows(nrows), ncols(ncols) {};

// Copy constructor
SparseMatrixCSR::SparseMatrixCSR(const SparseMatrixCSR& other) : nrows(other.nrows), ncols(other.ncols), values(other.values), cols(other.cols), row_idx(other.row_idx) {};

// Copy assignment
SparseMatrixCSR& SparseMatrixCSR::operator=(const SparseMatrixCSR& other) {
    if(!(this->nrows == other.nrows) || !(this->ncols == other.ncols)) {
        std::cout << "Error: Matrices have conflicting sizes, assignment avoided.\n";
        return *this;
    };

    this->values = other.values;
    this->cols = other.cols;
    this->row_idx = other.row_idx;

    return *this;
};

// Destructor
SparseMatrixCSR::~SparseMatrixCSR() {
    // No dynamic memory to free
};

const double& SparseMatrixCSR::operator()(const uint row, const uint col) const {
    // Find the row in the row_idx vector
    auto row_start = this->row_idx.at(row);
    auto row_end = this->row_idx.at(row + 1);

    // Search for the column in the current row's range
    for (auto i = row_start; i < row_end; ++i) {
        if (this->cols.at(i) == col) {
            return this->values.at(i);
        }
    }

    // If not found, return a reference to a static zero
    static const double zero = 0.0;
    return zero;
}

double& SparseMatrixCSR::operator()(const uint row, const uint col) {
    // Find the row in the row_idx vector
    auto row_start = this->row_idx.at(row);
    auto row_end = this->row_idx.at(row + 1);

    // Search for the column in the current row's range
    for (auto i = row_start; i < row_end; ++i) {
        if (this->cols.at(i) == col) {
            return this->values.at(i);
        }
    }

    // If not found, we cannot return a reference to a non-existent element
    throw std::out_of_range("Element not found in sparse matrix.");
};

SparseMatrixCSR& SparseMatrixCSR::operator*(const SparseMatrix& vec) const {
    // Placeholder implementation
    throw std::logic_error("SparseMatrix multiplication not implemented yet.");
}

SparseMatrixCSR& SparseMatrixCSR::operator*(const std::vector<double>& vec) const {
    // Placeholder implementation
    throw std::logic_error("SparseMatrix and vector multiplication not implemented yet.");
}

std::ostream& operator<<(std::ostream& os, const SparseMatrixCSR& mat) {
    os << "SparseMatrixCSR(" << mat.nrows << "x" << mat.ncols << ") with " << mat.get_nonzeros() << " non-zero elements.\n";
    os << "Values: ";
    for (const auto& val : mat.values) {
        os << val << " ";
    }
    os << "\nColumns: ";
    for (const auto& col : mat.cols) {
        os << col << " ";
    }
    os << "\nRow indices: ";
    for (const auto& idx : mat.row_idx) {
        os << idx << " ";
    }
    os << "\n";
    return os;
};