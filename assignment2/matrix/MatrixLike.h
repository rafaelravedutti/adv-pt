#pragma once

// forward declarations
template<typename T, std::size_t size_>
class Vector;

template<typename T, class Derived, size_t nrows, size_t ncols>
class MatrixLike {
public:
	/// c'tor/ d'tor
	virtual ~MatrixLike ( ) noexcept = 0; // pure virtual destructor

	/// virtual operators
	virtual Vector<T, nrows> operator* (const Vector<T, nrows> & o) const = 0;
	// feel free to extend as required

	/// other functions
	virtual Derived inverseDiagonal( ) const = 0;
	// feel free to extend as required

protected:
};

// MatrixLike d'tor implementation
template<typename T, class Derived, size_t nrows, size_t ncols>
inline MatrixLike<T, Derived, nrows, ncols>::~MatrixLike ( ) noexcept { }
