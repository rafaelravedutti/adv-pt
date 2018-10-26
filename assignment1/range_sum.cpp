#include <iostream>

int main() {
  int min, max, aux, sum;
  unsigned int i;

  std::cin >> min;
  std::cin >> max;

  sum = 0;

  if(min > max) {
    aux = min;
    min = max;
    max = aux;
  }

  for(i = min; i < max; ++i) {
    sum += i;
  }

  std::cout << sum << std::endl;
  return 0;
}
