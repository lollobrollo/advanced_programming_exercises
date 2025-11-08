#include "transientmatrixelement.hpp"
#include "sparse.hpp"

TransientMatrixElement::TransientMatrixElement(uint i, uint j, SparseMatrix& matrix) : locX(i), locY(j), matrix(matrix) {}

// Assignment operator
TransientMatrixElement& TransientMatrixElement::operator=(const double& value) {
    this->matrix.setValue(locX, locY, value);
    return *this;
}

// conversion to double for read access
TransientMatrixElement::operator double() const {
    return static_cast<const SparseMatrix&>(this->matrix)(locX, locY);
}