#include <iostream>

int main() {
  const unsigned int length1 = 10U, length2 = 12U;
  unsigned int sum = 0U;
  for( unsigned int i = 0U; i < length1-length2; ++i )
    sum += i;
  std::cout << "sum: " << sum << std::endl;

  return 0;
}
