
#include "sparsecoo.hpp"

// overloading typecasts
// https://www.learncpp.com/cpp-tutorial/overloading-typecasts/

typedef unsigned int uint;

// constructor
MyTuple::MyTuple(uint row, uint col, double val, SparseMatrixCOO& mat) : mat_entry(row, col, val), mat(mat) {
    this->mat.entries.push_back(*this); // save current entry in corresponding matrix
};

// copy constructor
MyTuple::MyTuple(MyTuple& other) : mat_entry(get<0>(other.mat_entry), get<1>(other.mat_entry), get<2>(other.mat_entry)), mat(other.mat) {};

// copy assignment
MyTuple& MyTuple::operator=(MyTuple& other) {
  // if(!(get<0>(other.mat_entry) == 0.0))
  if(this != other) {
    this->mat_entry = other.mat_entry;
  };
  return *this;
};