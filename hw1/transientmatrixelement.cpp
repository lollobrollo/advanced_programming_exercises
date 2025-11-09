#include "transientmatrixelement.hpp"
#include "sparse.hpp"

template<typename T> TransientMatrixElement<T>::TransientMatrixElement(uint i, uint j, SparseMatrix<T>& matrix) : locX(i), locY(j), matrix(matrix) {}

// Assignment operator
template<typename T> TransientMatrixElement<T>& TransientMatrixElement<T>::operator=(const T& value) {
    this->matrix.setValue(locX, locY, value);
    return *this;
}

// conversion to double for read access
template<typename T> TransientMatrixElement<T>::operator T() const {
    return static_cast<const SparseMatrix<T>&>(this->matrix)(locX, locY);
}

// Explicit instantiation for common types
template class TransientMatrixElement<double>;