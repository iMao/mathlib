#ifndef MAT_H
#define MAT_H

#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <type_traits>

namespace la {

#define MSG_LENGTH 256

class NotEqualMatSize : public std::exception {
  char warning_msg_[MSG_LENGTH]{};

 public:
  explicit NotEqualMatSize(const char* msg) { strcpy(warning_msg_, msg); }
  virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN
      _GLIBCXX_NOTHROW override {
    return warning_msg_;
  }
};

class IncorrectIndex : public std::exception {
  char warning_msg_[MSG_LENGTH]{};

 public:
  explicit IncorrectIndex(const char* msg) { strcpy(warning_msg_, msg); }
  virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN
      _GLIBCXX_NOTHROW override {
    return warning_msg_;
  }
};

///
/// \brief The Mat class - container for matrix with elements <T>
///
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
      : rows_(rows), cols_(cols), quantity_(rows * cols) {
    try {
      ptr_ = new T[quantity_];

      for (int i = 0; i < quantity_; i++) {
        ptr_[i] = val;
      }
    } catch (const std::bad_alloc& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  Mat(int rows, int cols, T* scope_ptr, int scope_size, T val = T{})
      : rows_(rows), cols_(cols), quantity_(rows * cols) {
    try {
      if (!scope_ptr) {
        throw "scope_ptr == nullptr";
      }

      if (scope_size < quantity_ * sizeof(T)) {
        throw "scope memory space is not enouph";
      }

      ptr_ = scope_ptr;

      for (int i = 0; i < quantity_; i++) {
        ptr_[i] = val;
      }

    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  Mat(const Mat& m) {
    int quantity = m.rows_ * m.cols_;
    try {
      ptr_ = new T[quantity];
    } catch (const std::bad_alloc& e) {
      std::cerr << e.what() << std::endl;
    }

    try {
      for (int i = 0; i < quantity; i++) {
        ptr_[i] = m.ptr_[i];
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    //---------Calba's line------------------
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = quantity;
  }

  Mat(Mat&& m) noexcept {
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = m.quantity_;
    ptr_ = m.ptr_;
    m.ptr_ = nullptr;
  }

  Mat& operator=(const Mat& m) {
    int quantity = m.rows_ * m.cols_;

    try {
      ptr_ = new T[quantity];
    } catch (const std::bad_alloc& e) {
      std::cerr << e.what() << std::endl;
    }

    try {
      for (int i = 0; i < quantity; i++) {
        ptr_[i] = m.ptr_[i];
      }
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    //-------Calba's line-------------------
    rows_ = m.rows_;
    cols_ = m.cols_;
    quantity_ = quantity;

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
        throw NotEqualMatSize("rows != cols");
      }
    } catch (const la::NotEqualMatSize& e) {
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
        throw IncorrectIndex("y - incorrect");
      }

      if (x >= cols_ || x < 0) {
        throw IncorrectIndex("x - incorrect");
      }

    } catch (const la::IncorrectIndex& e) {
      std::cerr << e.what() << std::endl;
    }

    return ptr_[y * cols_ + x];
  }

  static Mat CreateEye(int rows, int cols) {
    try {
      if (rows != cols) {
        throw NotEqualMatSize("rows != cols");
      }
    } catch (const la::NotEqualMatSize& e) {
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
