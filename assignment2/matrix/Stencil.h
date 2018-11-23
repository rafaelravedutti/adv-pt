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

  Stencil(Stencil && o) noexcept;

  ~Stencil( ) noexcept override { }

  Stencil& operator=(const Stencil & o) {
    boundaryStencil_ = o.boundaryStencil_;
    innerStencil_ = o.innerStencil_;
  }

  Stencil& operator=(Stencil && o) noexcept;

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
    auto boundary_entries = boundaryStencil_;
    auto inner_entries = innerStencil_;

    auto boundary_it = std::find_if(boundary_entries.begin(), boundary_entries.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    auto inner_it = std::find_if(inner_entries.begin(), inner_entries.end(),
      [] (StencilEntry<T> const &elem) {
        return elem.first == 0;
      }
    );

    boundary_it->second = 1.0 / boundary_it->second;
    inner_it->second = 1.0 / inner_it->second;

    return Stencil(boundary_entries, inner_entries);
  };

protected:
	// containers for the stencil entries -> boundary stencils represent the first and last rows of a corresponding
	// matrix and are to be applied to the first and last element of a target vector; inner stencils correspond to
	// the remaining rows of the matrix
	std::vector<StencilEntry<T> > boundaryStencil_;	// feel free to change the datatype if convenient
	std::vector<StencilEntry<T> > innerStencil_;	// feel free to change the datatype if convenient
};
