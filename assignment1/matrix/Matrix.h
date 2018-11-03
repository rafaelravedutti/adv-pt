#include <iostream>

class Matrix {
private:
  enum MatrixError {
    ERR_SUCCESS, ERR_DIM, ERR_OPER_DIM
  };

  std::size_t nrows, ncols;
  double *data;
  MatrixError error = MatrixError::ERR_SUCCESS;
public:

  Matrix(std::size_t rows, std::size_t cols, double initValue) : nrows(rows), ncols(cols) {
    if(rows * cols != 0) {
      data = new double[rows * cols];
    } else {
      data = nullptr;
      error = MatrixError::ERR_DIM;
    }

    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = initValue;
      }
    }
  }

  ~Matrix() {
    delete[] data;
  }

  Matrix(const Matrix& m) : nrows(m.rows()), ncols(m.cols()) {
    if(m.rows() * m.cols() != 0) {
      data = new double[m.rows() * m.cols()];
    } else {
      data = nullptr;
      error = MatrixError::ERR_DIM;
    }

    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        data[i * ncols + j] = m(i, j);
      }
    }
  }

  Matrix& operator=(const Matrix& m) {
    if(&m != this) {
      if(data != NULL) {
        delete[] data;
      }

      nrows = m.rows();
      ncols = m.cols();
      data = new double[nrows * ncols];

      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] = m(i, j);
        }
      }
    }

    return *this;
  }

  double& operator()(std::size_t i, std::size_t j) {
    return data[i * ncols + j];
  }

  const double& operator()(std::size_t i, std::size_t j) const {
    return data[i * ncols + j];
  }

  bool operator ==(const Matrix& m) const {
    if(nrows != m.nrows || ncols != m.cols()) {
      return false;
    }

    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        if(data[i * ncols + j] != m(i, j)) {
          return false;
        }
      }
    }

    return true;
  }

  bool operator !=(const Matrix& m) const {
    if(nrows != m.rows() || ncols != m.cols()) {
      return true;
    }

    for(std::size_t i = 0; i < nrows; ++i) {
      for(std::size_t j = 0; j < ncols; ++j) {
        if(data[i * ncols + j] != m(i, j)) {
          return true;
        }
      }
    }

    return false;

  }

  Matrix& operator +=(const Matrix& m) {
    if(nrows != m.nrows || ncols != m.cols()) {
      error = MatrixError::ERR_OPER_DIM;
    } else {
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] += m(i, j);
        }
      }
    }

    return *this;
  }

  Matrix operator +(const Matrix& m) const {
    Matrix result(nrows, ncols, 0.0);

    if(nrows != m.rows() || ncols != m.cols()) {
      return Matrix(0, 0, 0.0);
    } else {
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] + m(i, j);
        }
      }
    }

    return result;
  }

  Matrix& operator -=(const Matrix& m) {
    if(nrows != m.rows() || ncols != m.cols()) {
      error = MatrixError::ERR_OPER_DIM;
    } else {
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          data[i * ncols + j] -= m(i, j);
        }
      }
    }

    return *this;
  }

  Matrix operator -(const Matrix& m) const {
    Matrix result(nrows, ncols, 0.0);

    if(nrows != m.rows() || ncols != m.cols()) {
      return Matrix(0, 0, 0.0);
    } else {
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] - m(i, j);
        }
      }
    }

    return result;
  }

  Matrix& operator *=(const Matrix& m) {
    double *old_data = data;

    if(ncols != m.rows()) {
      error = MatrixError::ERR_OPER_DIM;
    } else {
      data = new double[nrows * m.cols()];

      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < m.cols(); ++j) {
          data[i * m.cols() + j] = 0.0;

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

  Matrix operator *(const Matrix& m) const {
    Matrix result(nrows, m.cols(), 0.0);

    if(ncols != m.rows()) {
      return Matrix(0, 0, 0.0);
    } else {
      for(std::size_t i = 0; i < nrows; ++i) {
        for(std::size_t j = 0; j < m.cols(); ++j) {
          for(std::size_t k = 0; k < ncols; ++k) {
            result(i, j) += data[i * ncols + k] * m(k, j);
          }
        }
      }
    }

    return result;
  }

  std::size_t rows() const {
    return nrows;
  }

  std::size_t cols() const {
    return ncols;
  }

  friend std::ostream& operator <<(std::ostream& output_stream, const Matrix& m) {
    for(std::size_t i = 0; i < m.rows(); ++i) {
      for(std::size_t j = 0; j < m.cols(); ++j) {
        output_stream << m(i, j) << " ";
      }

      output_stream << std::endl;
    }

    return output_stream;
  }

  friend std::istream& operator >>(std::istream& input_stream, Matrix& m) {
    for(std::size_t i = 0; i < m.rows(); ++i) {
      for(std::size_t j = 0; j < m.cols(); ++j) {
        input_stream >> m(i, j);
      }
    }

    return input_stream;
  }

  bool has_error() {
    return (error != MatrixError::ERR_SUCCESS);
  }

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
