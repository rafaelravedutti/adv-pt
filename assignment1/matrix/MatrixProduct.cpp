#include <iostream>
#include "Matrix.h"

int main() {
  /* Dimensions s1, s2 and s3 */
  std::size_t s1, s2, s3;

  /* Read dimensions from the standard input */
  std::cin >> s1 >> s2 >> s3;

  /* If one of the dimensions is zero, returns an error message */
  if(s1 * s2 * s3 == 0) {
    std::cerr << "None of the input dimensions should be zero!" << std::endl;
    return -1;
  }

  /* Define matrix m1, m2 and m3 with the specified dimensions */
  Matrix m1(s1, s2, 0.0);
  Matrix m2(s2, s3, 0.0);
  Matrix m3(s1, s3, 0.0);

  /* Read matrix m1 data from standard input */
  std::cin >> m1;
  /* Read matrix m2 data from standard input */
  std::cin >> m2;

  /* Assign the m1 and m2 product result to the matrix m3 */
  m3 = m1 * m2;

  /* If there's an error with the result matrix m3, returns it */
  if(m3.has_error()) {
    std::cerr << m3.error_message() << std::endl;
    return -1;
  }

  /* Show the result matrix m3 */
  std::cout << m3;
  return 0;
}
