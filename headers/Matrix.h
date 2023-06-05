#ifndef MATRIX_H_
#define MATRIX_H_

#define DefaultMatrixMeasure 1
#define VectorizeColsSize 1

#include <string>

class Matrix {
  int rows;
  int cols;
  int size;
  float *mat;
 public:
  /** Constructs matrix rows * cols. Initiates all elements to 0. */
  Matrix();
  /** Constructs 1*1 matrix, where the single element is initiated to 0. */
  Matrix(int rows, int cols);
  /** Constructs matrix from another matrix. */
  Matrix(const Matrix &m);
  /** Destroys the matrix. */
  ~Matrix();
  /** Returns the amount of rows */
  int GetRows() const noexcept;
  /** Returns the amount of columns */
  int GetCols() const noexcept;
  /** Transforms a matrix into a column vector. */
  Matrix &Vectorize() noexcept;
  /** Prints matrix elements. */
  void Print() noexcept;
  Matrix &operator=(const Matrix &rhs);
  Matrix operator*(const Matrix &rhs) const;
  Matrix operator*(float rhs) const noexcept;
  friend Matrix operator*(float c, const Matrix &rhs) noexcept;
  Matrix &operator*=(const Matrix &rhs);
  friend Matrix& operator*=(Matrix &lhs, float c);
  Matrix operator/(float rhs) const;
  Matrix &operator/=(float c);
  Matrix operator+(const Matrix &rhs) const;
  Matrix &operator+=(Matrix& rhs);
  Matrix &operator+=(float c) noexcept;
  bool operator==(const Matrix& rhs)const noexcept;
  bool operator!=(const Matrix& rhs)const noexcept;
  float &operator[](int i) const;
  float &operator()(int i, int j) const;
  float &operator[](int i) ;
  float &operator()(int i, int j) ;
  friend std::ostream &operator<<(std::ostream &os, const Matrix &rhs);
  friend std::istream &operator>>(std::istream &os, Matrix &rhs) ;
};

#endif //MATRIX_H_

