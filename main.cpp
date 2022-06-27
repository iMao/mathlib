#include <iostream>

#include "mat.h"

int main() {
  std::cout << "la::Mat test" << std::endl;

  la::Mat<int> m(3, 4, 5);

  std::cout << m << std::endl;
  m.negate();
  std::cout << m << std::endl;

  la::Mat<int> n = la::Mat<int>::CreateEye(4, 4);
  std::cout << n << std::endl;
  n.at(0, 1) = 3;
  n.at(0, 2) = 3;
  n.at(0, 3) = 3;

  n.at(1, 2) = 3;
  n.at(1, 3) = 3;

  n.at(2, 3) = 3;

  std::cout << n << std::endl;

  n.transpose();
  std::cout << n << std::endl;

  la::Mat<int> f(2, 3);
  f.at(0, 0) = 1;
  f.at(0, 1) = 2;
  f.at(0, 2) = 3;

  f.at(1, 0) = 1;
  f.at(1, 1) = 2;
  f.at(1, 2) = 3;

  std::cout << f << std::endl;

  la::Mat<int> tf(3, 2);

  f.transpose(tf);
  std::cout << tf << std::endl;

  la::Mat<float> flmat(3, 3, 3.14);
  std::cout << flmat << std::endl;

  flmat.dump(std::cout);

  std::string file_matrix{"float_mat.txt"};
  flmat.dump(file_matrix);

  return 0;
}
