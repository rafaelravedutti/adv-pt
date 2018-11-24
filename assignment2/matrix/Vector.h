#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>

#pragma once

template<typename T, std::size_t size_>
class Vector {
private:
  /* Data pointer */
  std::array<T, size_> data;
public:
  /* Vector constructor */
  Vector(T initValue) {
    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = initValue;
    }
  }

  /* Vector constructor */
  Vector(std::function<T(std::size_t)> initFunc) {
    /* Go through the data elements and fill all the positions with the
       given initial value */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = initFunc(i);
    }
  }

  /* Vector destructor */
  ~Vector() {}

  /* Vector copy constructor */
  Vector(const Vector<T, size_>& v) {
    /* Copy element by element from the given vector to this new one */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] = v(i);
    }
  }

  /* Vector assignment */
  Vector<T, size_>& operator=(const Vector<T, size_>& v) {
    /* Assure that if this vector is assigned to itself, nothing is done */
    if(&v != this) {
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
  bool operator ==(const Vector<T, size_>& v) const {
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
  bool operator !=(const Vector<T, size_>& v) const {
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
  Vector<T, size_>& operator +=(const Vector<T, size_>& v) {
    /* Go through each element of the vectors and perform the addition
       assignment for each element */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] += v(i);
    }

    return *this;
  }

  /* Vector addition */
  Vector<T, size_> operator +(const Vector<T, size_>& v) const {
    /* Result vector */
    Vector<T, size_> result(0.0);

    /* Go through each element of the vectors assign the sum of the elements 
       to the result vector */
    for(std::size_t i = 0; i < size_; ++i) {
      result(i) = data[i] + v(i);
    }

    return result;
  }

  /* Subtraction assignment operator */
  Vector<T, size_>& operator -=(const Vector<T, size_>& v) {
    /* Go through each element of the vectors and perform the subtraction
       assignment for each element */
    for(std::size_t i = 0; i < size_; ++i) {
      data[i] -= v(i);
    }

    return *this;
  }

  /* Vector subtraction */
  Vector<T, size_> operator -(const Vector<T, size_>& v) const {
    /* Result vector */
    Vector<T, size_> result(0.0);

    /* Go through each element of the vectors assign the subtraction of the
       elements to the result vector */
    for(std::size_t i = 0; i < size_; ++i) {
      result(i) = data[i] - v(i);
    }

    return result;
  }

  /* Product assignment operator */
  Vector<T, size_>& operator *=(const Vector<T, size_>& v) {
    /* Go through each element of the result vector */
    for(std::size_t i = 0; i < size_; ++i) {
      /* Perform the vector product */
      data[i] = data[i] * v(i);
    }

    return *this;
  }

  /* Vector product */
  Vector<T, size_> operator *(const Vector<T, size_>& v) const {
    /* Result vector */
    Vector<T, size_> result(0.0);

    /* Go through each element of the result vector */
    for(std::size_t i = 0; i < size_; ++i) {
      result(i) = data[i] * v(i);
    }

    return result;
  }

  /* Returns the L2 norm for the vector */
  double l2Norm() const {
    /* The norm is calculated by performing the summation of the square of
       each element in the vector, and then the square root of the summation */
    return sqrt(std::accumulate(
      data.begin(), data.end(), 0.0,
      [](T a, T b) { return a + (b * b); }
    ));
  }

  /* Return the number of rows */
  std::size_t size() const {
    return size_;
  }

  /* Input and output operators */
  template<typename vT, std::size_t vsize_>
  friend std::ostream& operator <<(std::ostream& output_stream, const Vector<vT, vsize_>& v);
  template<typename vT, std::size_t vsize_>
  friend std::istream& operator >>(std::istream& input_stream, Vector<vT, vsize_>& v);
};

/* Print vector data to the output stream */
template<typename T, std::size_t vsize>
std::ostream& operator <<(std::ostream& output_stream, const Vector<T, vsize>& v) {
  /* Go through each element and print it */
  for(std::size_t i = 0; i < vsize; ++i) {
    output_stream << v(i) << " ";
  }

  output_stream << std::endl;

  return output_stream;
}

/* Read vector data from the input stream */
template<typename T, std::size_t vsize>
std::istream& operator >>(std::istream& input_stream, Vector<T, vsize>& v) {
  /* Go through each element and read it */
  for(std::size_t i = 0; i < vsize; ++i) {
    input_stream >> v(i);
  }

  return input_stream;
}
