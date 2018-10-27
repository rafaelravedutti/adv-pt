#include <iostream>

int main() {
  unsigned int sum = 0U;
  for( unsigned int i=100U; i>=0U; --i )
    sum += i;
  std::cout << "sum: " << sum << std::endl;

  return 0;
}
