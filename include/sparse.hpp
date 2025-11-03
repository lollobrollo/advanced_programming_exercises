/*
Ah yes, is this the best way to share thoughts? Of course it is, it's the comments in the working file

Random stuff:
- regarding operator() for reading:
-> we might afford to copy indexes when accessing matrix elements? (avoid using & for inputs) 
-> return type should be a copy or a reference to the entries?

- return type of dot product (overloading * operator) should be a new object or reference? (I'm assuming vector x is instatiated as a SparseMatrix object)
- should we add vector for row and column indeces to the abstract class attributes? (depending on implementation their meaning changes slightly)
- decide what the constructor should take as input
-

Proposals:
- should we use getter and setter methods to access class attributes?
- 
- 

*/


#pragma once

#ifndef SPARSE_HPP
#define SPARSE_HPP

#include <vector>

class SparseMatrix {
  private:
    int nrows;
    int ncols;
    vector<double> values;
    // vector<int> rows;
    // vector<int> cols;

  public:
    virtual SparseMatrix() = 0; // constructor
    virtual SparseMatrix(const SparseMatrix& other) = 0; // copy constructor
    virtual SparseMatrix& operator=(const SparseMatrix& other) = 0; // copy assignment
    virtual ~SparseMatrix() = 0; // destructor
    
    virtual const double& operator()(int &row, int &col) const = 0; // read only (for const objects)
    virtual double& operator()(int &row, int &col) = 0; // write access (for non const objects)
    virtual SparseMatrix operator*(SparseMatrix& A, SparseMatrix& x) const = 0; // dot product 
    
    virtual const int get_nrows() const = 0;
    virtual const int get_ncols() const = 0;
    virtual const int n_nonzeros() const = 0;
    virtual void display_matrix() = 0; // print matrix to standard output
};


#endif // SPARSE_HPP