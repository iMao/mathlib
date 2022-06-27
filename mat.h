#ifndef MAT_H
#define MAT_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <type_traits>

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

    return *this;
  }

  Mat& transpose() & {
    try {
      if (rows_ != cols_) {
        throw "rows != cols";
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    int size = rows_;

    for (int y = 0; y < size; y++) {
      for (int x = y; x < size; x++) {
        std::swap(ptr_[y * size + x], ptr_[x * size + y]);
      }
    }
    return *this;
  }

  bool transpose(Mat& transposed_mat) const {
    if (transposed_mat.nrows() != cols_ || transposed_mat.ncols() != rows_) {
      return false;
    }

    for (int y = 0; y < rows_; y++) {
      for (int x = 0; x < cols_; x++) {
        transposed_mat.at(x, y) = ptr_[y * cols_ + x];
      }
    }

    return true;
  }

  void dump(std::ostream& os) const { os << *this; }

  void dump(std::ofstream& ofs) const { ofs << *this; }

  bool dump(const std::string& fname) const {
    std::ofstream ofs(fname, std::ios::out);
    if (!ofs.is_open()) {
      return false;
    }
    ofs << *this;
    ofs.close();
    return true;
  }

  ProxyRow operator[](int n) {
    ProxyRow prow{.row = &ptr_[n * cols_]};
    return prow;
  }
  const ProxyRow operator[](int n) const {
    ProxyRow prow{.row = &ptr_[n * cols_]};
    return prow;
  }

  T& at(int y, int x) & {
    try {
      if (y >= rows_ || y < 0) {
        throw "y - incorrect";
      }

      if (x >= cols_ || x < 0) {
        throw "x - incorrect";
      }

    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    return ptr_[y * cols_ + x];
  }

  static Mat CreateEye(int rows, int cols) {
    try {
      if (rows != cols) {
        throw "rows != cols";
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    Mat eye(rows, cols, static_cast<T>(0));

    for (int i = 0; i < rows; i++) {
      eye[i][i] = static_cast<T>(1);
    }
    return eye;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const la::Mat<T>& mat) {
  for (int y = 0; y < mat.nrows(); y++) {
    for (int x = 0; x < mat.ncols(); x++) {
      if (std::is_same<T, unsigned long>::value ||
          std::is_same<T, long>::value) {
        os << std::dec << std::setw(14);
      }

      if (std::is_same<T, int>::value || std::is_same<T, short>::value ||
          std::is_same<T, char>::value ||
          std::is_same<T, unsigned int>::value ||
          std::is_same<T, unsigned short>::value ||
          std::is_same<T, unsigned char>::value) {
        os << std::dec << std::setw(7);
      }

      if (std::is_same<T, float>::value) {
        os << std::setw(10) << std::setprecision(4);
      }

      if (std::is_same<T, double>::value) {
        os << std::setw(16) << std::setprecision(7);
      }

      os << mat[y][x] << " ";
    }
    os << std::endl;
  }
  return os;
}

template <typename T>
std::ofstream& operator<<(std::ofstream& ofs, const la::Mat<T>& mat) {
  for (int y = 0; y < mat.nrows(); y++) {
    for (int x = 0; x < mat.ncols(); x++) {
      if (std::is_same<T, unsigned long>::value ||
          std::is_same<T, long>::value) {
        ofs << std::dec << std::setw(14);
      }

      if (std::is_same<T, int>::value || std::is_same<T, short>::value ||
          std::is_same<T, char>::value ||
          std::is_same<T, unsigned int>::value ||
          std::is_same<T, unsigned short>::value ||
          std::is_same<T, unsigned char>::value) {
        ofs << std::dec << std::setw(7);
      }

      if (std::is_same<T, float>::value) {
        ofs << std::setw(10) << std::setprecision(4);
      }

      if (std::is_same<T, double>::value) {
        ofs << std::setw(16) << std::setprecision(7);
      }

      ofs << mat[y][x] << " ";
    }
    ofs << std::endl;
  }
  return ofs;
}

}  // namespace la

#endif  // MAT_H
