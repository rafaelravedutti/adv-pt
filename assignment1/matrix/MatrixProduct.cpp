#include <iostream>
#include "Matrix.h"

int main() {
  std::size_t s1, s2, s3;

  std::cin >> s1 >> s2 >> s3;

  if(s1 * s2 * s3 == 0) {
    std::cerr << "None of the input dimensions should be zero!" << std::endl;
    return -1;
  }

  Matrix m1(s1, s2, 0.0);
  Matrix m2(s2, s3, 0.0);
  Matrix m3(s1, s3, 0.0);

  std::cin >> m1;
  std::cin >> m2;

  m3 = m1 * m2;

  if(m3.has_error()) {
    std::cerr << m3.error_message() << std::endl;
    return -1;
  }

  std::cout << m3;
  return 0;
}
