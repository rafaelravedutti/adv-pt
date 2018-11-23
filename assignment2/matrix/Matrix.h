#include <iostream>
#include "Vector.h"
#include "MatrixLike.h"

#pragma once

template<typename T>
class Matrix : public MatrixLike<T, Matrix<T>> {
private:
  /* Possible errors for the matrix class */
  enum MatrixError {
    ERR_SUCCESS, ERR_DIM, ERR_OPER_DIM
  };

  /* Matrix dimensions */
  std::size_t nrows, ncols;
  /* Data pointer */
  T *data;
  /* Current error state */
  MatrixError error = MatrixError::ERR_SUCCESS;
public:

  /* Matrix constructor */
  Matrix(std::size_t rows, std::size_t cols, T initValue) : nrows(rows), ncols(cols) {
    /* If none of the dimensions is zero, allocate the data and assign it to
       the data pointer */
    if(rows * cols != 0) {
      data = new T[rows * cols];
    /* Otherwise, assign nullptr to the data pointer and change the error state
       to dimension error */
    } else {
      data = nullptr;
      error = MatrixError::ERR_DIM;
    }

    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = initValue;
      }
    }
  }

  /* Matrix destructor */
  ~Matrix() {
    /* Free memory in the data pointer */
    delete[] data;
  }

  /* Matrix copy constructor */
  Matrix(const Matrix<T>& m) : nrows(m.rows()), ncols(m.cols()) {
    /* If none of the dimensions is zero, allocate the data and assign it to
       the data pointer */
    if(m.rows() * m.cols() != 0) {
      data = new T[m.rows() * m.cols()];
    /* Otherwise, assign nullptr to the data pointer and change the error state
       to dimension error */
    } else {
      data = nullptr;
      error = MatrixError::ERR_DIM;
    }

    /* Copy element by element from the given matrix to this new one */
    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = m(i, j);
      }
    }
  }

  /* Matrix assignment */
  Matrix<T>& operator=(const Matrix<T>& m) {
    /* Assure that if this matrix is assigned to itself, nothing is done */
    if(&m != this) {
      /* Free data if it is not null */
      if(data != NULL) {
        delete[] data;
      }

      /* Set dimensions to the corresponding dimensions from the assigned matrix */
      nrows = m.rows();
      ncols = m.cols();
      /* Allocate new data according to the new dimensions */
      data = new T[nrows * ncols];

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
  bool operator ==(const Matrix<T>& m) const {
    /* If dimensions differ, return false */
    if(nrows != m.rows() || ncols != m.cols()) {
      return false;
    }

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

  /* Check if matrixes are different */
  bool operator !=(const Matrix<T>& m) const {
    /* If dimensions differ, return true */
    if(nrows != m.rows() || ncols != m.cols()) {
      return true;
    }

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

  /* Addition assignment operator */
  Matrix<T>& operator +=(const Matrix<T>& m) {
    /* If dimensions differ, do nothing and change the error state */
    if(nrows != m.rows() || ncols != m.cols()) {
      error = MatrixError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the matrixes and perform the addition
         assignment for each element */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] += m(i, j);
        }
      }
    }

    return *this;
  }

  /* Matrix addition */
  Matrix<T> operator +(const Matrix<T>& m) const {
    /* Result matrix */
    Matrix<T> result(nrows, ncols, 0.0);

    /* If dimensions differ, do nothing and returns an invalid matrix */
    if(nrows != m.rows() || ncols != m.cols()) {
      return Matrix<T>(0, 0, 0.0);
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the matrixes assign the sum of the elements 
         to the result matrix */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] + m(i, j);
        }
      }
    }

    return result;
  }

  /* Subtraction assignment operator */
  Matrix<T>& operator -=(const Matrix<T>& m) {
    /* If dimensions differ, do nothing and change the error state */
    if(nrows != m.rows() || ncols != m.cols()) {
      error = MatrixError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the matrixes and perform the subtraction
         assignment for each element */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] -= m(i, j);
        }
      }
    }

    return *this;
  }

  /* Matrix subtraction */
  Matrix<T> operator -(const Matrix<T>& m) const {
    /* Result matrix */
    Matrix<T> result(nrows, ncols, 0.0);

    /* If dimensions differ, do nothing and returns an invalid matrix */
    if(nrows != m.rows() || ncols != m.cols()) {
      return Matrix<T>(0, 0, 0.0);
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the matrixes assign the subtraction of the
         elements to the result matrix */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] - m(i, j);
        }
      }
    }

    return result;
  }

  /* Product assignment operator */
  Matrix<T>& operator *=(const Matrix<T>& m) {
    /* Pointer to old data */
    T *old_data = data;

    /* If the number of columns of this matrix and the number of rows of the
       other matrix is not equal, do nothing and change the error state */
    if(ncols != m.rows()) {
      error = MatrixError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Allocate data for the result with its dimensions */
      data = new T[nrows * m.cols()];

      /* Go through each element of the result matrix */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < m.cols(); ++j) {
          /* Current element of the result matrix is set to zero by default */
          data[i * m.cols() + j] = 0.0;

          /* Perform the matrix product by going through all the k columns in the
             current line i for the first matrix, and through all the k lines in the
             j column for the second matrix, the result is the summation of the
             products got on each iteration */
          for(std::size_t k = 0; k < ncols; ++k) {
            data[i * m.cols() + j] += old_data[i * ncols + k] * m(k, j);
          }
        }
      }

      ncols = m.cols();
      delete[] old_data;
    }

    return *this;
  }

  /* Matrix product */
  Matrix<T> operator *(const Matrix& m) const {
    /* Result matrix */
    Matrix<T> result(nrows, m.cols(), 0.0);

    /* If the number of columns of this matrix and the number of rows of the
       other matrix is not equal, do nothing and returns an invalid matrix */
    if(ncols != m.rows()) {
      return Matrix<T>(0, 0, 0.0);
    } else {
      /* Go through each element of the result matrix */
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < m.cols(); ++j) {
          /* Perform the matrix product by going through all the k columns in the
             current line i for the first matrix, and through all the k lines in the
             j column for the second matrix, the result is the summation of the
             products got on each iteration */
          for(std::size_t k = 0; k < ncols; ++k) {
            result(i, j) += data[i * ncols + k] * m(k, j);
          }
        }
      }
    }

    return result;
  }

  /* Matrix-Vector product */
  Vector<T> operator *(const Vector<T> &v) const {
    /* Result vector */
    Vector<T> result(nrows, 0.0);

    /* If the number of columns of this matrix and the number of rows of the
       vector is not equal, do nothing and returns an invalid vector */
    if(ncols != v.size()) {
      return Vector<T>(0, 0.0);
    } else {
      /* Go through each element of the result vector */
      for(std::size_t i = 0; i < nrows; ++i) {
        /* Perform the vector product by going through all the j columns in the
           current line i for the matrix, and through all the j lines in the
           vector, the result is the summation of the products of each iteration */
        for(std::size_t j = 0; j < ncols; ++j) {
          result(i) += data[i * ncols + j] * v(j);
        }
      }
    }

    return result;
  }

  /* Returns the inverse diagonal of the matrix */
  Matrix<T> inverseDiagonal() const {
    /* Result matrix */
    Matrix<T> result(nrows, ncols, 0.0);

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

  /* Print matrix data to the output stream */
  friend std::ostream& operator <<(std::ostream& output_stream, const Matrix<T>& m) {
    /* Go through each element and print it */
    for(std::size_t i = 0; i < m.rows(); ++i) {
      for(std::size_t j = 0; j < m.cols(); ++j) {
        output_stream << m(i, j) << " ";
      }

      output_stream << std::endl;
    }

    return output_stream;
  }

  /* Read matrix data from the input stream */
  friend std::istream& operator >>(std::istream& input_stream, Matrix<T>& m) {
    /* Go through each element and read it */
    for(std::size_t i = 0; i < m.rows(); ++i) {
      for(std::size_t j = 0; j < m.cols(); ++j) {
        input_stream >> m(i, j);
      }
    }

    return input_stream;
  }

  /* Check if matrix is in an error state */
  bool has_error() {
    return (error != MatrixError::ERR_SUCCESS);
  }

  /* Return the message for the matrix current error state */
  std::string error_message() {
    switch(error) {
      case MatrixError::ERR_SUCCESS:
        return "No error found!";
      case MatrixError::ERR_DIM:
        return "Matrix dimension is invalid!";
      case MatrixError::ERR_OPER_DIM:
        return "Problem with dimensions size during operation!";
      default:
        return "Some error occurred!";
    }
  }
};
