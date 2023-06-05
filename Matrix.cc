#include "Matrix.h"
#include "MatrixException.h"
#include <iostream>
#include <fstream>
#include <string>

#define ZERO 0
#define RECIPROCAL(c) 1/c
#define SUCCESSOR(c) c+1

Matrix::Matrix() {
  rows = DefaultMatrixMeasure;
  cols = DefaultMatrixMeasure;
  size = DefaultMatrixMeasure;
  mat = new float[size];
}

Matrix::Matrix(int rows, int cols) {
  this->rows = rows;
  this->cols = cols;
  this->size = (rows * cols);
  try {
    this->mat = new float[this->size]();
  } catch (std::bad_alloc &bad_alloc) {
    throw MatrixException("Allocation failed.\n");
  }
}

Matrix::Matrix(const Matrix &that) {
  this->rows = that.rows;
  this->cols = that.cols;
  this->size = that.size;
  try {
    this->mat = new float[this->size]();
    std::copy(that.mat, that.mat + that.size, this->mat);
  } catch (std::bad_alloc &bad_alloc) {
    throw MatrixException("Allocation failed.\n");
  }
}

Matrix::~Matrix() {
  delete[] this->mat;
}

int Matrix::GetRows() const noexcept{
  return this->rows;
}

int Matrix::GetCols() const noexcept{
  return this->cols;
}

Matrix &Matrix::Vectorize() noexcept{
  if (this->cols != VectorizeColsSize) {
    this->rows = (this->rows * this->cols);
    this->cols = VectorizeColsSize;
  }
  return *this;
}

void Matrix::Print() noexcept{
  for (int i = 0; i < this->rows; ++i) {
    for (int j = 0; j < this->cols; ++j) {
      std::cout << (*this).mat[(i * this->cols) + j] << ' ';
    }
    std::cout << std::endl;
  }
}

Matrix &Matrix::operator=(const Matrix &rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->rows = rhs.rows;
  this->cols = rhs.cols;
  this->size = rhs.size;
  try {
    delete[] this->mat;
    this->mat = new float[this->size]();
    std::copy(rhs.mat, rhs.mat + rhs.size, this->mat);
  } catch (std::bad_alloc &bad_alloc) {
    throw MatrixException("Allocation failed.\n");
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix &rhs) const {
  if (this->GetCols() != rhs.GetRows()){
    throw MatrixException("Invalid matrix dimensions.\n");
  }
  Matrix matrix = Matrix(this->rows, rhs.GetCols());
  float sum;
  for (int i = 0; i < this->GetRows(); ++i) {
    for (int j = 0; j < rhs.GetCols(); ++j) {
      sum = 0;
      for (int k = 0; k < this->GetCols(); ++k) {
        sum += this->mat[i * this->GetCols() + k] * rhs.mat[k * rhs.GetCols() + j];
      }
      matrix[i * matrix.GetCols() + j] = sum;
    }
  }
  return matrix;
}

Matrix Matrix::operator*(const float rhs) const noexcept{
  Matrix matrix = Matrix(*this);
  for (int i = 0; i < this->rows; ++i) {
    for (int j = 0; j < this->cols; ++j) {
      matrix.mat[(i * this->cols) + j] *= rhs;
    }
  }
  return matrix;
}

Matrix operator*(float c, const Matrix &rhs) noexcept{
  Matrix matrix = Matrix(rhs);
  for (int i = 0; i < rhs.rows; ++i) {
    for (int j = 0; j < rhs.cols; ++j) {
      matrix.mat[(i * rhs.cols) + j] *= c;
    }
  }
  return matrix;
}

Matrix &Matrix::operator*=(const Matrix &rhs) {
  *this = (*this)*(rhs);
  return *this;
}

Matrix &operator*=(Matrix &lhs, float c) {
  lhs = (lhs)*c;
  return lhs;
}

Matrix Matrix::operator/(float c) const {
  if (c == ZERO){
    throw MatrixException("Division by zero.\n");
  }
  return (*this)*(RECIPROCAL(c));
}

Matrix &Matrix::operator/=(float c) {
  if (c == ZERO){
    throw MatrixException("Division by zero.\n");
  }
  *this = (*this)/c;
  return *this;
}

Matrix Matrix::operator+(const Matrix &rhs) const {
  if ((this->GetRows() != rhs.GetRows()) || ((this->GetCols() != rhs.GetCols()))){
    throw MatrixException("Invalid matrix dimensions.\n");
  }
  Matrix sum = Matrix(rhs.GetRows(),rhs.GetCols());
  for (int i = 0; i < rhs.rows; ++i) {
    for (int j = 0; j < rhs.cols; ++j) {
      sum.mat[(i * rhs.cols) + j] = (this->mat[(i * rhs.cols) + j] + rhs.mat[(i * rhs.cols) + j]);
    }
  }
  return sum;
}

Matrix &Matrix::operator+=(Matrix &rhs) {
  if ((this->GetRows() != rhs.GetRows()) || ((this->GetCols() != rhs.GetCols()))){
    throw MatrixException("Invalid matrix dimensions.\n");
  }
  for (int i = 0; i < rhs.rows; ++i) {
    for (int j = 0; j < rhs.cols; ++j) {
      this->mat[(i * rhs.cols) + j] += rhs.mat[(i * rhs.cols) + j];
    }
  }
  return *this;
}

Matrix &Matrix::operator+=(float c) noexcept{
  for (int i = 0; i < this->rows; ++i) {
    for (int j = 0; j < this->cols; ++j) {
      this->mat[(i * this->cols) + j] += c;
    }
  }
  return *this;
}

bool Matrix::operator==(const Matrix &rhs) const noexcept{
  if (this == &rhs){
    return true;
  }
  if ((this->GetRows() != rhs.GetRows()) || ((this->GetCols() != rhs.GetCols()))){
    return false;
  }
  bool b = true;
  for (int i = 0; i < this->rows; ++i) {
    for (int j = 0; j < this->cols; ++j) {
      if ((this->mat[(i * this->cols) + j]) != (rhs.mat[(i * rhs.cols) + j])){
        b = false;
      }
    }
  }
  return b;
}

bool Matrix::operator!=(const Matrix &rhs) const noexcept{
  return !(*this == rhs);
}

float &Matrix::operator[](int i) const {
  if (this->size <= i){
    throw MatrixException("Index out of range.\n");
  }
  return this->mat[i];
}

float &Matrix::operator[](int i) {
  if (this->size <= i){
    throw MatrixException("Index out of range.\n");
  }
  return this->mat[i];
}

float &Matrix::operator()(int i, int j) const{
  if ((this->GetRows() <= i) || ((this->GetCols() <= j))){
    throw MatrixException("Index out of range.\n");
  }
  return this->mat[(i * this->cols) + j];
}

float &Matrix::operator()(int i, int j) {
  if ((this->GetRows() <= i) || ((this->GetCols() <= j))){
    throw MatrixException("Index out of range.\n");
  }
  return this->mat[(i * this->cols) + j];
}

std::ostream &operator<<(std::ostream &os, const Matrix &rhs) {
  if (!os.good()){
    throw MatrixException("Error writing to output stream.\n");
  }
  for (int i = 0; i < rhs.rows; ++i) {
    for (int j = 0; j < rhs.cols; ++j) {
      os << rhs.mat[(i * rhs.cols) + j] << ' ';
    }
    if (SUCCESSOR(i) != rhs.rows) {  // to avoid writing empty line at the end
      os << std::endl;
    }
  }
  return os;
}

std::istream &operator>>(std::istream &is, Matrix &rhs){
  if (!is.good()){
    throw MatrixException("Error loading from input stream.\n");
  }
  for (int i = 0; i < rhs.GetRows(); ++i) {
    for (int j = 0; j < rhs.GetCols(); ++j) {
      is >> rhs.mat[(i * rhs.GetCols()) + j];
    }
  }
  return is;
}
