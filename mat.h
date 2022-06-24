#ifndef MAT_H
#define MAT_H

#include <iostream>

namespace la {

template <typename T>
class Mat {
 private:
  struct ProxyRow {
    T* row;
    T& operator[](int n) { return row[n]; }
    const T& operator[](int n) const { return row[n]; }
  };

 private:
  int rows_;
  int cols_;
  int quantity_;
  T* ptr_;

 public:
  Mat() = delete;

  Mat(int rows, int cols, T val = T{})
      : rows_(rows), cols_(cols), quantity_(rows_ * cols_) {
    try {
      ptr_ = new T[quantity_];

      for (int i = 0; i < quantity_; i++) {
        ptr_[i] = val;
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  Mat(const Mat& m) {
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = m.quantity_;
    try {
      ptr_ = new T[quantity_];
      for (int i = 0; i < quantity_; i++) {
        ptr_[i] = m.ptr_[i];
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  Mat(Mat&& m) noexcept {
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = m.quantity_;
    ptr_ = m.ptr_;
    m.ptr_ = nullptr;
  }

  Mat& operator=(const Mat& m) {
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = m.quantity_;

    try {
      ptr_ = new T[quantity_];
      for (int i = 0; i < quantity_; i++) {
        ptr_[i] = m.ptr_[i];
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    return *this;
  }

  Mat& operator=(Mat&& m) noexcept {
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = m.quantity_;
    ptr_ = m.ptr_;
    m.ptr_ = nullptr;

    return *this;
  }

  ~Mat() {
    if (ptr_ != nullptr) {
      delete[] ptr_;
    }
  }

  int nrows() const { return rows_; }

  int ncols() const { return cols_; }

  int nquantity() const { return quantity_; }

  bool equal(const Mat& m) const {
    bool ret = true;
    for (int i = 0; i < quantity_; i++) {
      if (ptr_[i] != m.ptr_[i]) {
        ret = false;
        break;
      }
    }
    return ret;
  }

  Mat& negate() & {
    for (int i = 0; i < quantity_; i++) {
      ptr_[i] = -(ptr_[i]);
    }
  }

  Mat& transpose() & { return *this; }

  void dump(std::ostream& os) const {}

  ProxyRow operator[](int n) {
    ProxyRow prow{.row = &ptr_[n * cols_]};
    return prow;
  }
  const ProxyRow operator[](int n) const {
    ProxyRow prow{.row = &ptr_[n * cols_]};
    return prow;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const la::Mat<T>& mat) {
  for (int y = 0; y < mat.nrows(); y++) {
    for (int x = 0; x < mat.ncols(); x++) {
      os << mat[y][x] << " ";
    }
    os << std::endl;
  }
  return os;
}

}  // namespace la

#endif  // MAT_H
