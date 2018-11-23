#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>

#pragma once

template<typename T>
class Vector {
private:
  /* Possible errors for the vector class */
  enum VectorError {
    ERR_SUCCESS, ERR_DIM, ERR_OPER_DIM
  };

  /* Vector dimensions */
  std::size_t size_;
  /* Data pointer */
  T *data;
  /* Current error state */
  VectorError error = VectorError::ERR_SUCCESS;
public:

  /* Vector constructor */
  Vector(std::size_t elements, T initValue) : size_(elements) {
    /* If none of the dimensions is zero, allocate the data and assign it to
       the data pointer */
    if(size_ != 0) {
      data = new T[size_];
    /* Otherwise, assign nullptr to the data pointer and change the error state
       to dimension error */
    } else {
      data = nullptr;
      error = VectorError::ERR_DIM;
    }

    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = initValue;
    }
  }

  /* Vector constructor */
  Vector(std::size_t elements, std::function<T(std::size_t)> initFunc) : size_(elements) {
    /* If none of the dimensions is zero, allocate the data and assign it to
       the data pointer */
    if(size_ != 0) {
      data = new T[size_];
    /* Otherwise, assign nullptr to the data pointer and change the error state
       to dimension error */
    } else {
      data = nullptr;
      error = VectorError::ERR_DIM;
    }

    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = initFunc(i);
    }
  }

  /* Vector destructor */
  ~Vector() {
    /* Free memory in the data pointer */
    delete[] data;
  }

  /* Vector copy constructor */
  Vector(const Vector<T>& v) : size_(v.size()) {
    /* If none of the dimensions is zero, allocate the data and assign it to
       the data pointer */
    if(size_ != 0) {
      data = new T[size_];
    /* Otherwise, assign nullptr to the data pointer and change the error state
       to dimension error */
    } else {
      data = nullptr;
      error = VectorError::ERR_DIM;
    }

    /* Copy element by element from the given vector to this new one */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = v(i);
    }
  }

  /* Vector assignment */
  Vector<T>& operator=(const Vector<T>& v) {
    /* Assure that if this vector is assigned to itself, nothing is done */
    if(&v != this) {
      /* Free data if it is not null */
      if(data != NULL) {
        delete[] data;
      }

      /* Set dimensions to the corresponding dimensions from the assigned vector */
      size = v.size();
      /* Allocate new data according to the new dimensions */
      data = new T[size];

      /* Copy element by element from the given vector to this one */
      for(std::size_t i = 0; i < size_; ++i) {
        data[i] = v(i);
      }
    }

    return *this;
  }

  /* Return reference from the specified index */
  T& operator()(std::size_t i) {
    return data[i];
  }

  /* Return element value from the specified index */
  const T& operator()(std::size_t i) const {
    return data[i];
  }

  /* Check if vectors are equal */
  bool operator ==(const Vector<T>& v) const {
    /* If dimensions differ, return false */
    if(size_ != v.size()) {
      return false;
    }

    /* Go through each element of the vector and in case one element differs
       from the other vector, returns false */
    for(std::size_t i = 0; i < size; ++i) {
      if(data[i] != v(i)) {
        return false;
      }
    }

    return true;
  }

  /* Check if vectors are different */
  bool operator !=(const Vector<T>& v) const {
    /* If dimensions differ, return true */
    if(size_ != v.size()) {
      return true;
    }

    /* Go through each element of the vector and in case one element differs
       from the other vector, returns true */
    for(std::size_t i = 0; i < size_; ++i) {
      if(data[i] != v(i)) {
        return true;
      }
    }

    return false;

  }

  /* Addition assignment operator */
  Vector<T>& operator +=(const Vector<T>& v) {
    /* If dimensions differ, do nothing and change the error state */
    if(size_ != v.size()) {
      error = VectorError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the vectors and perform the addition
         assignment for each element */
      for(std::size_t i = 0; i < size_; ++i) {
        data[i] += v(i);
      }
    }

    return *this;
  }

  /* Vector addition */
  Vector<T> operator +(const Vector<T>& v) const {
    /* Result vector */
    Vector<T> result(size_, 0.0);

    /* If dimensions differ, do nothing and returns an invalid vector */
    if(size_ != v.size()) {
      return Vector<T>(0, 0.0);
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the vectors assign the sum of the elements 
         to the result vector */
      for(std::size_t i = 0; i < size_; ++i) {
        result(i) = data[i] + v(i);
      }
    }

    return result;
  }

  /* Subtraction assignment operator */
  Vector<T>& operator -=(const Vector<T>& v) {
    /* If dimensions differ, do nothing and change the error state */
    if(size_ != v.size()) {
      error = VectorError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the vectors and perform the subtraction
         assignment for each element */
      for(std::size_t i = 0; i < size_; ++i) {
        data[i] -= v(i);
      }
    }

    return *this;
  }

  /* Vector subtraction */
  Vector<T> operator -(const Vector<T>& v) const {
    /* Result vector */
    Vector<T> result(size_, 0.0);

    /* If dimensions differ, do nothing and returns an invalid vector */
    if(size_ != v.size()) {
      return Vector<T>(0, 0.0);
    /* Otherwise, proceed normally */
    } else {
      /* Go through each element of the vectors assign the subtraction of the
         elements to the result vector */
      for(std::size_t i = 0; i < size_; ++i) {
        result(i) = data[i] - v(i);
      }
    }

    return result;
  }

  /* Product assignment operator */
  Vector<T>& operator *=(const Vector<T>& v) {
    /* Pointer to old data */
    T *old_data = data;

    /* If the number of columns of this vector and the number of rows of the
       other vector is not equal, do nothing and change the error state */
    if(size_ != v.size()) {
      error = VectorError::ERR_OPER_DIM;
    /* Otherwise, proceed normally */
    } else {
      /* Allocate data for the result with its dimensions */
      data = new T[size_];

      /* Go through each element of the result vector */
      for(std::size_t i = 0; i < size_; ++i) {
        /* Perform the vector product */
        data[i] = old_data[i] * v(i);
      }

      delete[] old_data;
    }

    return *this;
  }

  /* Vector product */
  Vector<T> operator *(const Vector<T>& v) const {
    /* Result vector */
    Vector<T> result(size_, 0.0);

    /* If the number of columns of this vector and the number of rows of the
       other vector is not equal, do nothing and returns an invalid vector */
    if(size_ != v.size()) {
      return Vector<T>(0, 0.0);
    } else {
      /* Go through each element of the result vector */
      for(std::size_t i = 0; i < size_; ++i) {
        result(i) = data[i] * v(i);
      }
    }

    return result;
  }

  /* Returns the L2 norm for the vector */
  double l2Norm() const {
    /* The norm is calculated by performing the summation of the square of
       each element in the vector, and then the square root of the summation */
    return sqrt(std::accumulate(
      data, data + size_, 0.0,
      [](T a, T b) { return a + (b * b); }
    ));
  }

  /* Return the number of rows */
  std::size_t size() const {
    return size_;
  }

  /* Print vector data to the output stream */
  friend std::ostream& operator <<(std::ostream& output_stream, const Vector<T>& v) {
    /* Go through each element and print it */
    for(std::size_t i = 0; i < v.size(); ++i) {
      output_stream << v(i) << " ";
    }

    output_stream << std::endl;

    return output_stream;
  }

  /* Read vector data from the input stream */
  friend std::istream& operator >>(std::istream& input_stream, Vector<T>& v) {
    /* Go through each element and read it */
    for(std::size_t i = 0; i < v.size(); ++i) {
      input_stream >> v(i);
    }

    return input_stream;
  }

  /* Check if vector is in an error state */
  bool has_error() {
    return (error != VectorError::ERR_SUCCESS);
  }

  /* Return the message for the vector current error state */
  std::string error_message() {
    switch(error) {
      case VectorError::ERR_SUCCESS:
        return "No error found!";
      case VectorError::ERR_DIM:
        return "Vector dimension is invalid!";
      case VectorError::ERR_OPER_DIM:
        return "Problem with dimensions size during operation!";
      default:
        return "Some error occurred!";
    }
  }
};
