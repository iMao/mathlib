#include <iostream>

#include "mat.h"

int main() {
  std::cout << "la::Mat test" << std::endl;

  la::Mat<int> m(3, 4, 5);

  std::cout << m;

  return 0;
}
