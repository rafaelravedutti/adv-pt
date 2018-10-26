#include <iostream>
#include <cctype>

int main() {
  std::string line;
  int i;

  std::getline(std::cin, line);

  for(i = 0; i < line.size(); ++i) {
    if(ispunct(line[i])) {
      line.erase(i--, 1);
    }
  }

  std::cout << line << std::endl;
  return 0;
}
