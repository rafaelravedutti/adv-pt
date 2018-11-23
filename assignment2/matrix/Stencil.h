#pragma once

#include <cassert>
#include <utility> //std::swap
#include <algorithm> // std::find_if
#include <vector>

#include "MatrixLike.h"

template<typename T>
using StencilEntry = std::pair<int, T>; // convenience type for stencil entries

template<typename T>
class Stencil : public MatrixLike<T, Stencil<T>> {
public:
  Stencil(const std::vector<StencilEntry<T> >& boundaryEntries, const std::vector<StencilEntry<T> >& innerEntries)
    : boundaryStencil_(boundaryEntries), innerStencil_(innerEntries) { }
  Stencil(const std::vector<StencilEntry<T> >& innerEntries)	// c'tor for stencils w/o explicit boundary handling
    : boundaryStencil_(innerEntries), innerStencil_(innerEntries) { }

  Stencil(const Stencil & o) : boundaryStencil_(o.boundaryStencil_), innerStencil_(o.innerStencil_) {
  }

  Stencil(Stencil && o) : boundaryStencil_(o.boundaryStencil_), innerStencil_(o.innerStencil_) {};

  ~Stencil( ) noexcept override { }

  Stencil& operator=(const Stencil & o) {
    boundaryStencil_ = o.boundaryStencil_;
    innerStencil_ = o.innerStencil_;
  }

  Stencil& operator=(Stencil && o) {
    boundaryStencil_ = o.boundaryStencil_;
    innerStencil_ = o.innerStencil_;
  }

	// HINT: stencil entries are stored as offset/coefficient pair, that is the offset specifies which element of a
	// vector, relative to the current index, is to be regarded. It is then multiplied with the according coefficient.
	// All of these expressions are evaluated and then summed up to get the final result.
  Vector<T> operator* (const Vector<T> & o) const {
    Vector<T> result(o.size(), 0.0);

    for(auto elem : boundaryStencil_) {
      result(0) += o(elem.first) * elem.second;
      result(o.size() - 1) += o(o.size() - 1 + elem.first) * elem.second;
    }

    for(std::size_t i = 1; i < o.size() - 1; ++i) {
      for(auto elem : innerStencil_) {
        result(i) += o(i + elem.first) * elem.second;
      }
    }

    return result;
  }

  Stencil<T> inverseDiagonal( ) const {
    auto boundary_it = std::find_if(boundaryStencil_.begin(), boundaryStencil_.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    auto inner_it = std::find_if(innerStencil_.begin(), innerStencil_.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    return Stencil({{0, 1.0 / boundary_it->second}}, {{0, 1.0 / inner_it->second}});
  };

  friend std::ostream& operator <<(std::ostream& output_stream, const Stencil<T>& st) {
    std::cout << "BOUNDARY = {";
    for(auto elem : st.boundaryStencil_) {
      std::cout << "<" << elem.first << ", " << elem.second << ">";
    }
    std::cout << "}" << std::endl;

    std::cout << "INNER = {";
    for(auto elem : st.innerStencil_) {
      std::cout << "<" << elem.first << ", " << elem.second << ">";
    }
    std::cout << "}" << std::endl;

    return output_stream;
  }

protected:
	// containers for the stencil entries -> boundary stencils represent the first and last rows of a corresponding
	// matrix and are to be applied to the first and last element of a target vector; inner stencils correspond to
	// the remaining rows of the matrix
	std::vector<StencilEntry<T> > boundaryStencil_;	// feel free to change the datatype if convenient
	std::vector<StencilEntry<T> > innerStencil_;	// feel free to change the datatype if convenient
};
