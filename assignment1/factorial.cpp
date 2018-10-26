#include <iostream>

int main() {
  int input;
  int i;
  double result;

  std::cin >> input;

  if(input < 0) {
    std::cout << "Input number must be positive!" << std::endl;
    return 1;
  }

  result = 1.0;

  for(i = input; i > 0; --i) {
    result *= i;
  }

  std::cout << result << std::endl;
  return 0;
}
