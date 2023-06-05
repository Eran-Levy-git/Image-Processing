#include "Filters.h"
#include <cmath>

#define SHADES float(256)

/**
 * Performs quantization on the input image by the given number of levels.
 * @param image matrix.
 * @param levels a power of 2 not bigger than 256.
 * @return new matrix which is the result of running the operator on the image.
 */
Matrix Quantization(const Matrix &image, int levels) {
  float shades_in_one_level;
  shades_in_one_level = SHADES / float(levels);
  auto *levels_array = new float[levels + 1];
  auto *mean_levels_array = new float[levels];
  // building the levels array
  for (int i = 0; i < levels + 1; ++i) {
    levels_array[i] = float(i) * shades_in_one_level;
  }
  // building the mean levels array
  for (int i = 0; i < levels; ++i) {
    mean_levels_array[i] = floorf((levels_array[i] + (levels_array[i + 1] - 1)) / 2);
  }
  // Quantifying
  Matrix quant = Matrix(image);
  for (int i = 0; i < quant.GetRows(); ++i) {
    for (int j = 0; j < quant.GetCols(); ++j) {
      for (int k = 0; k < levels + 1; ++k) {
        if ((levels_array[k] <= image(i, j)) and (image(i, j) < levels_array[k + 1])) {
          quant(i, j) = mean_levels_array[k];
        }
      }
    }
  }
  delete[] levels_array;
  delete[] mean_levels_array;
  return quant;
}

float CalcConvCell(int i, int j, Matrix &doctor, Matrix &patient_padded) {
  float sum;
  sum = ((patient_padded(i - 1, j - 1) * doctor(0, 0)) + (patient_padded(i - 1, j) * doctor(0, 1))
      + (patient_padded(i - 1, j + 1) * doctor(0, 2))
      + (patient_padded(i, j - 1) * doctor(1, 0)) + (patient_padded(i, j) * doctor(1, 1))
      + (patient_padded(i, j + 1) * doctor(1, 2))
      + (patient_padded(i + 1, j - 1) * doctor(2, 0)) + (patient_padded(i + 1, j) * doctor(2, 1))
      + (patient_padded(i + 1, j + 1) * doctor(2, 2)));
  return sum;
}

Matrix Convolution(const Matrix &patient, Matrix &doctor, bool is_sobel) {
  Matrix conv = Matrix(patient);
  // building padded matrix
  Matrix patient_padded = Matrix(patient.GetRows() + 2, patient.GetCols() + 2);
  for (int i = 0; i < patient.GetRows(); ++i) {
    for (int j = 0; j < patient.GetCols(); ++j) {
      patient_padded(i + 1, j + 1) = patient(i, j);
    }
  }
  // calculating convolution
  for (int i = 0; i < conv.GetRows(); ++i) {
    for (int j = 0; j < conv.GetCols(); ++j) {
      if (is_sobel){
        conv(i, j) = rintf(CalcConvCell(i + 1, j + 1, doctor, patient_padded));
      } else {
        conv(i, j) = CalcConvCell(i + 1, j + 1, doctor, patient_padded);
      }
    }
  }
  return conv;
}

void AdjustMatrix(Matrix &p_matrix) {
  for (int i = 0; i < p_matrix.GetRows(); ++i) {
    for (int j = 0; j < p_matrix.GetCols(); ++j) {
      if ((p_matrix)(i, j) > 255) {
        (p_matrix)(i, j) = 255;
      } else if ((p_matrix)(i, j) < 0) {
        (p_matrix)(i, j) = 0;
      }
      (p_matrix)(i, j) = rintf((p_matrix)(i, j));
    }
  }
}

/**
 * Performs gaussian blurring on the input image.
 * @param image is a 3x3 matrix.
 * @return new matrix which is the result of running the operator on the image.
 */
Matrix Blur(const Matrix &image) {
  // building the gaussian blurring matrix
  Matrix gauss = Matrix(3, 3);
  gauss(0, 0) = 1, gauss(0, 1) = 2, gauss(0, 2) = 1;
  gauss(1, 0) = 2, gauss(1, 1) = 4, gauss(1, 2) = 2;
  gauss(2, 0) = 1, gauss(2, 1) = 2, gauss(2, 2) = 1;
  gauss *= (float(1) / 16);
  // blurring the image
  Matrix blur = Convolution(image, gauss, false);
  AdjustMatrix(blur);
  return blur;
}

Matrix Sobel(const Matrix &image) {
// building the G_x matrix
  Matrix g_x = Matrix(3, 3);
  g_x(0, 0) = 1, g_x(0, 1) = 0, g_x(0, 2) = -1;
  g_x(1, 0) = 2, g_x(1, 1) = 0, g_x(1, 2) = -2;
  g_x(2, 0) = 1, g_x(2, 1) = 0, g_x(2, 2) = -1;
  g_x *= (float(1) / 8);
  // building the G_y matrix
  Matrix g_y = Matrix(3, 3);
  g_y(0, 0) = 1, g_y(0, 1) = 2, g_y(0, 2) = 1;
  g_y(1, 0) = 0, g_y(1, 1) = 0, g_y(1, 2) = 0;
  g_y(2, 0) = -1, g_y(2, 1) = -2, g_y(2, 2) = -1;
  g_y *= (float(1) / 8);
// blurring the image
  Matrix first = Convolution(image, g_x, true);
  Matrix second = Convolution(image, g_y, true);
  Matrix sobel = first + second;
  AdjustMatrix(sobel);
  return sobel;
}
