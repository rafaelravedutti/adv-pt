#include <iostream>
#include "Vector.h"
#include "MatrixLike.h"

#pragma once

template<typename T, std::size_t nrows, std::size_t ncols>
class Matrix : public MatrixLike<T, Matrix<T, nrows, ncols>, nrows, ncols> {
private:
  /* Data pointer */
  std::array<T, nrows * ncols> data;
public:
  /* Matrix constructor */
  Matrix(T initValue) {
    /* Check for matrix dimensions */
    static_assert(
      nrows >= 0 && ncols >= 0,
      "Matrix dimensions must be higher or equal than zero");

    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = initValue;
      }
    }
  }

  /* Matrix destructor */
  ~Matrix() {}

  /* Matrix copy constructor */
  Matrix(const Matrix<T, nrows, ncols>& m) {
    /* Copy element by element from the given matrix to this new one */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = m(i, j);
      }
    }
  }

  /* Matrix assignment */
  Matrix<T, nrows, ncols>& operator=(const Matrix<T, nrows, ncols>& m) {
    /* Assure that if this matrix is assigned to itself, nothing is done */
    if(&m != this) {
      /* Copy element by element from the given matrix to this one */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] = m(i, j);
        }
      }
    }

    return *this;
  }

  /* Return reference from the specified index */
  inline T& operator()(int i, int j) {
    return data[i * ncols + j];
  }

  /* Return element value from the specified index */
  inline const T& operator()(int i, int j) const {
    return data[i * ncols + j];
  }

  /* Check if matrixes are equal */
  bool operator ==(const Matrix<T, nrows, ncols>& m) const {
    /* Go through each element of the matrix and in case one element differs
       from the other matrix, returns false */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        if(data[i * ncols + j] != m(i, j)) {
          return false;
        }
      }
    }

    return true;
  }

  /* Check if matrixes are equal */
  template<std::size_t mnrows, std::size_t mncols>
  bool operator ==(const Matrix<T, mnrows, mncols>& m) const {
    return false;
  }

  /* Check if matrixes are different */
  bool operator !=(const Matrix<T, nrows, ncols>& m) const {
    /* Go through each element of the matrix and in case one element differs
       from the other matrix, returns true */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        if(data[i * ncols + j] != m(i, j)) {
          return true;
        }
      }
    }

    return false;

  }

  /* Check if matrixes are different */
  template<std::size_t mnrows, std::size_t mncols>
  bool operator !=(const Matrix<T, mnrows, mncols>& m) const {
    return true;
  }

  /* Addition assignment operator */
  Matrix<T, nrows, ncols>& operator +=(const Matrix<T, nrows, ncols>& m) {
    /* Go through each element of the matrixes and perform the addition
       assignment for each element */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] += m(i, j);
      }
    }

    return *this;
  }

  /* Matrix addition */
  Matrix<T, nrows, ncols> operator +(const Matrix<T, nrows, ncols>& m) const {
    /* Result matrix */
    Matrix<T, nrows, ncols> result(0.0);

    /* Go through each element of the matrixes assign the sum of the elements 
       to the result matrix */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        result(i, j) = data[i * ncols + j] + m(i, j);
      }
    }

    return result;
  }

  /* Subtraction assignment operator */
  Matrix<T, nrows, ncols>& operator -=(const Matrix<T, nrows, ncols>& m) {
    /* Go through each element of the matrixes and perform the subtraction
       assignment for each element */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] -= m(i, j);
      }
    }

    return *this;
  }

  /* Matrix subtraction */
  Matrix<T, nrows, ncols> operator -(const Matrix<T, nrows, ncols>& m) const {
    /* Result matrix */
    Matrix<T, nrows, ncols> result(0.0);

    /* Go through each element of the matrixes assign the subtraction of the
       elements to the result matrix */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        result(i, j) = data[i * ncols + j] - m(i, j);
      }
    }

    return result;
  }

  /* Product assignment operator */
  template<std::size_t mncols>
  Matrix<T, nrows, mncols>& operator *=(const Matrix<T, ncols, mncols>& m) {
    std::array<T, nrows * ncols> old_data(data);

    /* Go through each element of the result matrix */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < mncols; ++j) {
        /* Current element of the result matrix is set to zero by default */
        data[i * mncols + j] = 0.0;

        /* Perform the matrix product by going through all the k columns in the
           current line i for the first matrix, and through all the k lines in the
           j column for the second matrix, the result is the summation of the
           products got on each iteration */
        for(std::size_t k = 0; k < ncols; ++k) {
          data[i * mncols + j] += old_data[i * ncols + k] * m(k, j);
        }
      }
    }

    return *this;
  }

  /* Matrix product */
  template<std::size_t mncols>
  Matrix<T, nrows, mncols> operator *(const Matrix<T, ncols, mncols>& m) const {
    /* Result matrix */
    Matrix<T, nrows, mncols> result(0.0);

    /* Go through each element of the result matrix */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < mncols; ++j) {
        /* Perform the matrix product by going through all the k columns in the
           current line i for the first matrix, and through all the k lines in the
           j column for the second matrix, the result is the summation of the
           products got on each iteration */
        for(std::size_t k = 0; k < ncols; ++k) {
          result(i, j) += data[i * ncols + k] * m(k, j);
        }
      }
    }

    return result;
  }

  /* Matrix-Vector product */
  Vector<T, nrows> operator *(const Vector<T, nrows> &v) const {
    /* Result vector */
    Vector<T, nrows> result(0.0);

    /* Go through each element of the result vector */
    for(std::size_t i = 0; i < nrows; ++i) {
      /* Perform the vector product by going through all the j columns in the
         current line i for the matrix, and through all the j lines in the
         vector, the result is the summation of the products of each iteration */
      for(std::size_t j = 0; j < ncols; ++j) {
        result(i) += data[i * ncols + j] * v(j);
      }
    }

    return result;
  }

  /* Returns the inverse diagonal of the matrix */
  Matrix<T, nrows, ncols> inverseDiagonal() const {
    /* Result matrix */
    Matrix<T, nrows, ncols> result(0.0);

    /* Go through each element of the matrix diagonal and change their value
       in the result matrix */
    for(std::size_t i = 0; i < nrows; ++i) {
      result(i, i) = 1.0 / data[i * ncols + i];
    }

    return result;
  }

  /* Return the number of rows */
  std::size_t rows() const {
    return nrows;
  }

  /* Return the number of columns */
  std::size_t cols() const {
    return ncols;
  }

  /* Input and output operators */
  template<typename mT, std::size_t mnrows, std::size_t mncols>
  friend std::ostream& operator <<(std::ostream& output_stream, const Matrix<mT, mnrows, mncols>& m);
  template<typename mT, std::size_t mnrows, std::size_t mncols>
  friend std::istream& operator >>(std::istream& input_stream, Matrix<mT, mnrows, mncols>& m);
};


/* Print matrix data to the output stream */
template<typename T, std::size_t nrows, std::size_t ncols>
std::ostream& operator <<(std::ostream& output_stream, const Matrix<T, nrows, ncols>& m) {
  /* Go through each element and print it */
  for(std::size_t i = 0; i < nrows; ++i) {
    for(std::size_t j = 0; j < ncols; ++j) {
      output_stream << m(i, j) << " ";
    }

    output_stream << std::endl;
  }

  return output_stream;
}

/* Read matrix data from the input stream */
template<typename T, std::size_t nrows, std::size_t ncols>
std::istream& operator >>(std::istream& input_stream, Matrix<T, nrows, ncols>& m) {
  /* Go through each element and read it */
  for(std::size_t i = 0; i < nrows; ++i) {
    for(std::size_t j = 0; j < ncols; ++j) {
      input_stream >> m(i, j);
    }
  }

  return input_stream;
}
