#include <iostream>
#include <chrono>

#include "Matrix.h"
#include "Vector.h"
#include "Stencil.h"

#define PI 3.141592653589793

template<typename T, class MatrixImpl, std::size_t numGridPoints>
void solve (
  const MatrixLike<T, MatrixImpl, numGridPoints, numGridPoints>& A,
  const Vector<T, numGridPoints>& b,
  Vector<T, numGridPoints>& u) {

	double initRes = (b - A * u).l2Norm( ); // determine the initial residual
	double curRes = initRes;
	std::cout << "Initial residual:\t\t" << initRes << std::endl;

	unsigned int curIt = 0; // store the current iteration index

	while (curRes > 1.e-5 * initRes) { // solve until the residual is reduced by a certain amount
		++curIt;

		u += A.inverseDiagonal( ) * (b - A * u); // Jacobi step

		curRes = (b - A * u).l2Norm( ); // update the residual

		if (0 == curIt % 500) // print some info every few steps
			std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl;
	}

	std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl << std::endl; // print the final number of iterations and the final residual
}

template<std::size_t numGridPoints>
void testFullMatrix () {
  const double hx = 1. / (numGridPoints - 1);
  const double hxSq = hx * hx;

  std::cout << "Starting full matrix solver for " << numGridPoints << " grid points" << std::endl;

  Matrix<double, numGridPoints, numGridPoints> A(0.);
  Vector<double, numGridPoints> u(0.);
  Vector<double, numGridPoints> b([] (size_t x) {
    return sin(2. * PI * (x / (double)(numGridPoints - 1)));
  });

  A(0, 0) = 1.;
  for (std::size_t x = 1; x < numGridPoints - 1; ++x) {
    A(x, x - 1) = 1. / hxSq;
    A(x, x) = -2. / hxSq;
    A(x, x + 1) = 1. / hxSq;
  }
  A(numGridPoints - 1, numGridPoints - 1) = 1.;

  std::cout << "Initialization complete\n";

  auto start = std::chrono::system_clock::now();
  solve(A, b, u);
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
}

template<std::size_t numGridPoints>
void testStencil () {
  const double hx = 1. / (numGridPoints - 1);
  const double hxSq = hx * hx;

  std::cout << "Starting stencil solver for " << numGridPoints << " grid points" << std::endl;

  Stencil<double, numGridPoints, numGridPoints> ASten(
    { { 0, 1. } },
    { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } }
  );

  Vector<double, numGridPoints> u(0.);
  Vector<double, numGridPoints> b([] (size_t x) {
    return sin(2. * PI * (x / (double)(numGridPoints - 1)));
  });

  std::cout << "Initialization complete\n";

  auto start = std::chrono::system_clock::now();
  solve(ASten, b, u);
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << "s" << std::endl;
}

int main(int argc, char** argv) {
	testFullMatrix<32>();
	testStencil<32>();

	testFullMatrix<17>();
	testStencil<17>();

	testFullMatrix<33>();
	testStencil<33>();

	testFullMatrix<65>();
	testStencil<65>();

	testFullMatrix<129>();
	testStencil<129>();
}
