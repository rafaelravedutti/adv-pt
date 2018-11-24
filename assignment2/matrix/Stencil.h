#pragma once

#include <cassert>
#include <utility> //std::swap
#include <algorithm> // std::find_if
#include <vector>

#include "MatrixLike.h"

template<typename T>
using StencilEntry = std::pair<int, T>; // convenience type for stencil entries

template<typename T, std::size_t nrows, std::size_t ncols>
class Stencil : public MatrixLike<T, Stencil<T, nrows, ncols>, nrows, ncols> {
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
  Vector<T, nrows> operator* (const Vector<T, nrows> & o) const {
    /* Result vector */
    Vector<T, nrows> result(0.0);

    /* Go through each pair of the boundary entries */
    for(auto elem : boundaryStencil_) {
      /* Apply stencil for start boundary (position 0) */
      result(0) += o(elem.first) * elem.second;
      /* Apply stencil for end boundary (position nrows - 1) */
      result(nrows - 1) += o(nrows - 1 + elem.first) * elem.second;
    }

    /* Go through each element of the vector */
    for(std::size_t i = 1; i < nrows - 1; ++i) {
      /* Go through each pair of the inner entries */
      for(auto elem : innerStencil_) {
        /* Apply the stencil for current vector element */
        result(i) += o(i + elem.first) * elem.second;
      }
    }

    return result;
  }

  Stencil<T, nrows, ncols> inverseDiagonal( ) const {
    /* Find boundary pair where the first element (offset) is zero */
    auto boundary_it = std::find_if(boundaryStencil_.begin(), boundaryStencil_.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    /* Find inner pair where the first element (offset) is zero */
    auto inner_it = std::find_if(innerStencil_.begin(), innerStencil_.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    /* Return stencil with inverse values of the zero offsets (diagonal) */
    return Stencil({{0, 1.0 / boundary_it->second}}, {{0, 1.0 / inner_it->second}});
  };

protected:
	// containers for the stencil entries -> boundary stencils represent the first and last rows of a corresponding
	// matrix and are to be applied to the first and last element of a target vector; inner stencils correspond to
	// the remaining rows of the matrix
	std::vector<StencilEntry<T> > boundaryStencil_;	// feel free to change the datatype if convenient
	std::vector<StencilEntry<T> > innerStencil_;	// feel free to change the datatype if convenient
};
