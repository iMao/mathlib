#include "mat.h"

namespace la {

///
/// \brief add - sum of src1 and src2 matrix
/// \param m
/// \param n
/// \param dst - result of sum
/// \return true if all OK, and false if sizes of matrix incorrect
///
template <typename T>
bool add(const Mat<T> &m, const Mat<T> &n, Mat<T> &dst) {
  if (m.nrows() != n.nrows() || m.ncols() != n.ncols()) {
    return false;
  }

  if (m.nrows() != dst.nrows() || m.ncols() != dst.ncols()) {
    return false;
  }

  try {
    for (int y = 0; y < m.nrows(); y++) {
      for (int x = 0; x < m.ncols(); x++) {
        dst.at(y, x) = m.at(y, x) + n.at(y, x);
      }
    }
  } catch (const la::IncorrectIndex &e) {
    std::cerr << e.what() << std::endl;
  }

  return true;
}

///
/// \brief mul multiplication of matrices
/// \param m
/// \param n
/// \param dst
/// \return
///
template <typename T>
bool mul(const Mat<T> &m, const Mat<T> &n, Mat<T> &dst) {
  if (m.ncols() != n.nrows()) {
    return false;
  }

  if (dst.nrows() != m.nrows() || dst.ncols() != n.ncols()) {
    return false;
  }

  for (int i = 0; i < m.nrows(); i++) {
    for (int j = 0; j < n.ncols(); j++) {
      for (int k = 0; k < m.ncols(); k++) {
        dst.at(i, j) += m.at(i, k) * n.at(k, j);
      }
    }
  }

  return true;
}

}  // namespace la
