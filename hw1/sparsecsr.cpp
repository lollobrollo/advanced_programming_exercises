#include "sparsecsr.hpp"
#include "sparsecoo.hpp"
#include "transientmatrixelement.hpp"
#include <iostream>

template <typename T> uint SparseMatrixCSR<T>::get_nrows() const {
    return this->nrows;
};

template <typename T> uint SparseMatrixCSR<T>::get_ncols() const {
    return this->ncols;
};

template <typename T> uint SparseMatrixCSR<T>::get_nonzeros() const {
    return this->values.size();
};

// Parametrized constructor
template <typename T> SparseMatrixCSR<T>::SparseMatrixCSR(const uint nrows, const uint ncols) : nrows(nrows), ncols(ncols) {
    this->row_idx = std::vector<uint>(nrows + 1, 0);
    std::cout << "Empty matrix of dimensions " << this->nrows << " x " << this->ncols << " initialized.\n";
};

// Copy constructor
template <typename T> SparseMatrixCSR<T>::SparseMatrixCSR(const SparseMatrixCSR<T>& other) : nrows(other.nrows), ncols(other.ncols), values(other.values), cols(other.cols), row_idx(other.row_idx) {};

// Copy constructor from COO
template <typename T> SparseMatrixCSR<T>::SparseMatrixCSR(const SparseMatrixCOO<T>& other) : nrows(other.get_nrows()), ncols(other.get_ncols()) {
    // Convert COO to CSR
    this->values.clear();
    this->cols.clear();
    this->row_idx = std::vector<uint>(nrows + 1, 0);

    // Fill values and cols
    for (size_t i = 0; i < other.values.size(); ++i) {
        this->setValue(other.rows.at(i), other.cols.at(i), other.values.at(i));
    }
}

// Copy assignment
template <typename T> SparseMatrixCSR<T>& SparseMatrixCSR<T>::operator=(const SparseMatrixCSR<T>& other) {
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
template <typename T> SparseMatrixCSR<T>::~SparseMatrixCSR() {
    // No dynamic memory to free
};

template <typename T> T SparseMatrixCSR<T>::operator()(const uint row, const uint col) const {
    if((row >= this->nrows) || (col >= this->ncols)) {
        std::cout << "Error: assigning value out of matrix bounds. Assignment avoided.\n";
        return 0.0;
    }
    // Find the row in the row_idx vector
    auto row_start = this->row_idx.at(row);
    auto row_end = this->row_idx.at(row + 1);

    // Search for the column in the current row's range
    for (auto i = row_start; i < row_end; ++i) {
        if (this->cols.at(i) == col) {
            return this->values.at(i);
        }
    }

    return 0.0; // Element not found, returning default
}

template <typename T> TransientMatrixElement<T> SparseMatrixCSR<T>::operator()(const uint row, const uint col) {
    if((row >= this->nrows) || (col >= this->ncols)) {
        throw std::out_of_range("Matrix indices out of bounds");
    }
    return TransientMatrixElement<T>(row, col, *this);
};

template <typename T> void SparseMatrixCSR<T>::setValue(const uint row, const uint col, const T value) {
    if (row >= nrows || col >= ncols) throw std::out_of_range("index");

    // Find the row in the row_idx vector
    auto row_start = this->row_idx.at(row);
    auto row_end = this->row_idx.at(row + 1);

    // Search for the column in the current row's range
    for (auto i = row_start; i < row_end; ++i) {
        if (this->cols.at(i) == col) {
            if (value != 0.0) {
                this->values.at(i) = value; // Update existing value
            } else {
                // Remove the element
                this->values.erase(this->values.begin() + i);
                this->cols.erase(this->cols.begin() + i);
                // Update row_idx
                for (auto j = row + 1; j < this->row_idx.size(); ++j) {
                    this->row_idx.at(j)--;
                }
            }
            return;
        }
    }

    // If we reach here, the element does not exist
    if (value != 0.0) {
        // Insert new element
        this->values.insert(this->values.begin() + row_end, value);
        this->cols.insert(this->cols.begin() + row_end, col);
        // Update row_idx
        for (auto j = row + 1; j < this->row_idx.size(); ++j) {
            this->row_idx.at(j)++;
        }
    }
}

// SparseMatrixCSR SparseMatrixCSR::operator*(const SparseMatrixCSR& vec) const {
//     // Placeholder implementation
//     throw std::logic_error("SparseMatrix multiplication not implemented yet.");
// }


template <typename T> std::vector<T> SparseMatrixCSR<T>::operator*(const std::vector<T>& vec) const {
    std::vector<T> results(this->nrows, T{}); // initialize result vector with zeros

    for (uint row = 0; row < nrows; ++row) { // compute one at a time (row by row)
        uint row_start = row_idx.at(row);
        uint row_end = row_idx.at(row + 1); // assuming row_idx has size nrows+1
        for (uint idx = row_start; idx < row_end; ++idx) {
            results.at(row) += values.at(idx) * vec.at(this->cols.at(idx)); // compute results iteratively
        };
    };

    return results;
};

template <typename T> std::ostream& operator<<(std::ostream& os, const SparseMatrixCSR<T>& mat) {
    os << "SparseMatrixCSR(" << mat.nrows << "x" << mat.ncols << ") with " << mat.get_nonzeros() << " non-zero elements.\n";
    os << "Values: ";
    for (const auto& val : mat.values) {
        os << val << " ";
    };
    os << "\nColumns: ";
    for (const auto& col : mat.cols) {
        os << col << " ";
    };
    os << "\nRow indices: ";
    for (const auto& idx : mat.row_idx) {
        os << idx << " ";
    };
    os << "\n";
    return os;
};

// Explicit template instantiation
template class SparseMatrixCSR<double>;
template std::ostream& operator<<(std::ostream& os, const SparseMatrixCSR<double>& mat);