#include <iostream>

class Matrix {
private:
  double *data;
  std::size_t nrows, ncols;
public:
  Matrix(std::size_t rows, std::size_t cols, double initValue) : nrows(rows), ncols(cols) {
    data = new double[rows * cols];

    for(int i = 0; i < nrows; ++i) {
      for(int j = 0; j < ncols; ++j) {
        data[i * ncols + j] = initValue;
      }
    }
  }

  ~Matrix() {
    delete[] data;
  }

  Matrix(const Matrix& m) {
    nrows = m.rows();
    ncols = m.cols();
    data = new double[nrows * ncols];

    for(int i = 0; i < nrows; ++i) {
      for(int j = 0; j < ncols; ++j) {
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

      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < ncols; ++j) {
          data[i * ncols + j] = m(i, j);
        }
      }
    }
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

    for(int i = 0; i < nrows; ++i) {
      for(int j = 0; j < ncols; ++j) {
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

    for(int i = 0; i < nrows; ++i) {
      for(int j = 0; j < ncols; ++j) {
        if(data[i * ncols + j] != m(i, j)) {
          return true;
        }
      }
    }

    return false;

  }

  Matrix& operator +=(const Matrix& m) {
    if(nrows != m.nrows || ncols != m.cols()) {
      std::cout << "Cannot perform summation of matrix with different dimensions!" << std::endl;
    } else {
      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < ncols; ++j) {
          data[i * ncols + j] += m(i, j);
        }
      }
    }

    return *this;
  }

  Matrix operator +(const Matrix& m) const {
    Matrix result(nrows, ncols, 0.0);

    if(nrows != m.rows() || ncols != m.cols()) {
      std::cout << "Cannot perform summation of matrix with different dimensions!" << std::endl;
    } else {
      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] + m(i, j);
        }
      }
    }

    return result;
  }

  Matrix& operator -=(const Matrix& m) {
    if(nrows != m.rows() || ncols != m.cols()) {
      std::cout << "Cannot perform subtraction of matrix with different dimensions!" << std::endl;
    } else {
      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < ncols; ++j) {
          data[i * ncols + j] -= m(i, j);
        }
      }
    }

    return *this;
  }

  Matrix operator -(const Matrix& m) const {
    Matrix result(nrows, ncols, 0.0);

    if(nrows != m.rows() || ncols != m.cols()) {
      std::cout << "Cannot perform subtraction of matrix with different dimensions!" << std::endl;
    } else {
      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < ncols; ++j) {
          result(i, j) = data[i * ncols + j] - m(i, j);
        }
      }
    }

    return result;
  }

  Matrix& operator *=(const Matrix& m) {
    double *old_data = data;

    if(ncols != m.rows()) {
      std::cout << "Number of columns for the first matrix and rows for the second matrix must be the same to perform product!" << std::endl;
    } else {
      data = new double[nrows * m.cols()];

      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < m.cols(); ++j) {
          data[i * m.cols() + j] = 0.0;

          for(int k = 0; k < ncols; ++k) {
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
      std::cout << "Number of columns for the first matrix and rows for the second matrix must be the same to perform product!" << std::endl;
    } else {
      for(int i = 0; i < nrows; ++i) {
        for(int j = 0; j < m.cols(); ++j) {
          for(int k = 0; k < ncols; ++k) {
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
    for(int i = 0; i < m.rows(); ++i) {
      for(int j = 0; j < m.cols(); ++j) {
        output_stream << m(i, j) << " ";
      }

      output_stream << std::endl;
    }

    return output_stream;
  }

  friend std::istream& operator >>(std::istream& input_stream, Matrix& m) {
    for(int i = 0; i < m.rows(); ++i) {
      for(int j = 0; j < m.cols(); ++j) {
        input_stream >> m(i, j);
      }
    }

    return input_stream;
  }
};
