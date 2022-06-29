#ifndef MAT_EXCEPTIONS
#define MAT_EXCEPTIONS

#include <string.h>

#include <stdexcept>

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

class IncorrectScopeSpace : public std::exception {
  char warning_msg_[MSG_LENGTH]{};

 public:
  explicit IncorrectScopeSpace(const char* msg) { strcpy(warning_msg_, msg); }
  virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN
      _GLIBCXX_NOTHROW override {
    return warning_msg_;
  }
};

}  // namespace la

#endif
