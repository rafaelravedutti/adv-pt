#include <iostream>
#include <functional>
#include <chrono>

double measureTime(std::function<void()> toMeasure) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	toMeasure();
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	return elapsed.count();
}



using FacType = long long int;
using LoopItType = unsigned long long int;
const LoopItType numReps = 256ll * 1024 * 1024;

const int init = 16;

FacType factorial(FacType n) {
	FacType res = 1;
	for (LoopItType i = 1; i <= n; ++i)
		res *= i;
	return res;
}

FacType factorial_rec(FacType n) {
	if (0 == n)
		return 1;
	else
		return n * factorial(n - 1);
}

template <FacType N> struct Factorial {
	static const FacType result = N * Factorial<N - 1>::result;
};

template <> struct Factorial<0> {
	static const FacType result = 1;
};



void printFacLoop() {
	volatile FacType fac = init;
	volatile FacType res = 0;
	for (LoopItType i = 0; i < numReps; ++i)
		res = factorial(fac);
	std::cout << "The factorial of " << init << " is " << res << std::endl;
}
void printFacRec() {
	volatile FacType fac = init;
	volatile FacType res = 0;
	for (LoopItType i = 0; i < numReps; ++i)
		res = factorial_rec(fac);
	std::cout << "The factorial of " << init << " is " << res << std::endl;
}
void printFacTMP() {
	volatile FacType res = 0;
	for (LoopItType i = 0; i < numReps; ++i)
		res = Factorial<init>::result;
	std::cout << "The factorial of " << init << " is " << res << std::endl;
}



int main(int argc, char** argv) {
	std::cout << "Using a loop required: "    << measureTime(printFacLoop) << std::endl;
	std::cout << "Using recursion required: " << measureTime(printFacRec)  << std::endl;
	std::cout << "Using TMP required: "       << measureTime(printFacTMP)  << std::endl;

}
