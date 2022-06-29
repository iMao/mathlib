#include "mat.h"

namespace la {

inline const double TOLLERANCE{0.0001};

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
        if ((std::numeric_limits<T>::max() - n.at(y, x)) >= m.at(y, x))
          dst.at(y, x) = m.at(y, x) + n.at(y, x);
        else
          dst.at(y, x) = std::numeric_limits<T>::max();
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
        if ((std::numeric_limits<T>::max() / n.at(k, j)) <= m.at(i, k)) {
          return false;
        }

        T mul_result = m.at(i, k) * n.at(k, j);

        if ((std::numeric_limits<T>::max() - mul_result) <= dst.at(i, j)) {
          return false;
        }
        dst.at(i, j) += mul_result;
      }
    }
  }

  return true;
}

template <typename T>
double determinant(Mat<T> &A) {
  int n = A.ncols();

  double det = 1.0;

  // Row operations for i = 0, ,,,, n - 2 (n-1 not needed)
  for (int i = 0; i < n - 1; i++) {
    // Partial pivot: find row r below with largest element in column i
    int r = i;
    double maxA = std::abs(A.at(i, i));
    for (int k = i + 1; k < n; k++) {
      double val = std::abs(A.at(k, i));
      if (val > maxA) {
        r = k;
        maxA = val;
      }
    }
    if (r != i) {
      for (int j = i; j < n; j++) {
        std::swap(A.at(i, j), A.at(r, j));
      }
      det = -det;
    }

    // Row operations to make upper-triangular
    double pivot = A.at(i, i);
    // check if Singular matrix
    if (std::abs(pivot) < TOLLERANCE) {
      return (0.0);
    }

    // On lower rows
    for (int r = i + 1; r < n; r++) {
      // Multiple of row i to clear element in ith column
      double multiple = A.at(r, i) / pivot;
      for (int j = i; j < n; j++) {
        A.at(r, j) -= multiple * A.at(i, j);
      }
    }
    det *= pivot;  // Determinant is product of diagonal
  }

  det *= A.at(n - 1, n - 1);

  return det;
}

template <typename T>
double Determinant(const Mat<T> &B) {
  Mat<T> A(B);
  return determinant(A);
}

}  // namespace la
