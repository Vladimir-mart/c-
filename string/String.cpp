#include "String.hpp"

using std::cin;
using std::cout;

void String::Swap(String& other) {
  char* temp = this->str_;
  this->str_ = other.str_;
  other.str_ = temp;
}

std::istream& operator>>(std::istream& in, String& str_) {
  str_.Clear();
  char ch;
  while (in >> ch) {
    str_.PushBack(ch);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const String& str_) {
  for (size_t i = 0; i < str_.Size(); ++i) {
    out << str_[i];
  }
  return out;
}

String& String::operator=(const char* k_other) {
  if (this->size_ > 0) {
    delete[] str_;
  }
  int len = strlen(k_other);
  this->str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str_[i] = k_other[i];
  }
  this->str_[len] = '\0';
  this->capacity_ = len + 1;
  this->size_ = len;
  return *this;
}

String& String::operator=(const String& other) {
  if (this->str_ == other.Data()) {
    return *this;
  }
  if (this->size_ > 0) {
    delete[] this->str_;
  }
  int len = other.Size();
  this->str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str_[i] = other.str_[i];
  }
  this->str_[len] = '\0';
  this->capacity_ = len + 1;
  this->size_ = len;
  return *this;
}

bool String::operator>(const String& str_) const { return str_ < *this; }

bool String::operator<=(const String& str_) const { return !(*this > str_); }

bool String::operator>=(const String& str_) const { return !(*this < str_); }

bool String::operator==(const String& str_) const {
  return !(*this < str_ || *this > str_);
}

bool String::operator!=(const String& str_) const { return !(*this == str_); }

bool String::operator<(const String& str_) const {
  for (size_t i = 0; i < min(this->size_, str_.Size()); ++i) {
    if (this->str_[i] < str_[i]) {
      return this->str_[i] < str_[i];
    }
  }
  return (this->size_ < str_.Size());
}

void String::ShrinkToFit() {
  if (size_ < capacity_ - 1) {
    char* strt = new char[this->size_ + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    delete[] this->str_;
    this->str_ = strt;
    this->capacity_ = this->size_ + 1;
    this->str_[this->size_] = '\0';
  }
}

void String::Reserve(size_t new_cap) {
  if (this->capacity_ < new_cap) {
    char* strt = new char[new_cap * 2 + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    delete[] this->str_;
    this->str_ = strt;
    this->capacity_ = 2 * new_cap + 1;
    this->str_[this->size_] = '\0';
  }
}

void String::Resize(size_t new_size, char elem) {
  if (new_size >= this->capacity_) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    for (size_t j = 0; j < new_size - this->size_; j++, i++) {
      strt[i] = elem;
    }
    if (this->size_ > 0) {
      delete[] this->str_;
    }
    this->str_ = strt;
    this->size_ = new_size;
    this->capacity_ = new_size + 1;
    this->str_[this->size_] = '\0';
  } else {
    this->size_ = new_size;
    this->str_[new_size] = '\0';
  }
}

void String::Resize(size_t new_size) {
  if (new_size >= this->capacity_) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    if (this->size_ > 0) {
      delete[] this->str_;
    }
    this->str_ = strt;
    this->size_ = new_size;
    this->capacity_ = new_size + 1;
    this->str_[this->size_] = '\0';
  } else {
    this->size_ = new_size;
    this->str_[new_size] = '\0';
  }
}

// String& String::operator*=(int m) {
//     // cout <<  " ========== " ;
//     String t = this->str_;
//     // cout << t << " ========== " ;
//      int temp = m/10;
//      int tos = m%10;
//     if  (m > 1) {
//         for (int i = 1 ; i < 10; ++i) {
//             t += t;
//         }
//         for (int i = 0 ; i < temp; ++i) {
//             t += t;
//         }
//     }
//     for (int i = 1 ; i < tos; ++i) {
//         t += t;
//     }
//     // if (this->str_ != nullptr) {
//     //     delete[] this->str_;
//     // }
//      this->str_ = t.str_;
//     // return *this;
// }

String& String::operator*=(int m) {
  (*this) = (*this) * m;
  return *this;
}

String operator*(String str_, int m) {
  String result = "";
  if (m == 0) {
    return result;
  }
  if (m == 1) {
    result = str_;
  } else {
    result = str_ * (m / 2);
    result += result;
    if (m % 2 == 1) {
      result += str_;
    }
  }
  return result;
}

String& String::operator+=(const char kOther) {
  PushBack(kOther);
  return *this;
}

String& String::operator+=(const char* other) {
  size_t len = strlen(other);

  if (this->capacity_ <= (this->size_ + len + 1)) {
    char* strt = new char[this->size_ + len + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    for (size_t j = 0; j < len; j++, i++) {
      strt[i] = other[j];
    }
    if (this->size_ > 0) {
      delete[] this->str_;
    }
    this->str_ = strt;
    this->size_ = len + this->size_;
    this->capacity_ = this->size_ + 1;
    this->str_[this->size_] = '\0';
  } else {
    for (size_t i = this->size_, j = 0; j < len; ++i, ++j) {
      this->str_[i] = other[j];
    }
    this->size_ += len;
    this->str_[this->size_ + 1] = '\0';
  }
  return *this;
}

String& String::operator+=(const String& other) {
  size_t len = other.Size();

  if (this->capacity_ <= (this->size_ + len + 1)) {
    char* strt = new char[this->size_ + len + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    for (size_t j = 0; j < len; j++, i++) {
      strt[i] = other.str_[j];
    }
    delete[] this->str_;
    this->str_ = strt;
    this->size_ = len + this->size_;
    this->capacity_ = this->size_ + 1;
    this->str_[this->size_] = '\0';
  } else {
    for (size_t i = this->size_, j = 0; j < len; ++i, ++j) {
      this->str_[i] = other.str_[j];
    }
    this->size_ += len;
    this->str_[this->size_ + 1] = '\0';
  }
  return *this;
}

String operator+(const String& other1, const String& other2) {
  String result = other1;
  result += other2;
  return result;
}

void String::PopBack() {
  if (this->size_ == 0) {
    return;
  }
  --size_;
}

size_t String::Capacity() const { return this->capacity_ - 1; }

void String::PushBack(const char kElem) {
  if (this->size_ == (this->capacity_ - 1)) {
    int len = this->size_ * 2;
    char* strt = new char[len + 1];
    for (size_t i = 0; i < size_; ++i) {
      strt[i] = str_[i];
    }
    delete[] this->str_;
    this->str_ = strt;
    this->str_[size_] = kElem;
    this->size_ = size_ + 1;
    this->capacity_ = len + 1;
    this->str_[size_] = '\0';
  } else {
    this->str_[size_] = kElem;
    this->size_ = size_ + 1;
    this->str_[size_] = '\0';
  }
}

size_t String::Size() const { return this->size_; }

void String::Clear() { this->size_ = 0; }

const char* String::Data() const {
  this->str_[this->size_] = '\0';
  return str_;
}

String::String(const size_t kSiz, const char& sim) {
  this->str_ = new char[kSiz + 1];
  for (size_t i = 0; i < kSiz; ++i) {
    str_[i] = sim;
  }
  this->str_[kSiz] = '\0';
  this->capacity_ = kSiz + 1;
  this->size_ = kSiz;
}

String::String() {
  this->str_ = nullptr;
  this->capacity_ = 1;
}

String::String(const String& str_) {
  size_t len = str_.Size();
  this->str_ = new char[len + 1];
  for (size_t i = 0; i < len; ++i) {
    this->str_[i] = str_.str_[i];
  }
  this->str_[len] = '\0';
  this->capacity_ = len + 1;
  this->size_ = len;
}

String::String(const char* str_) {
  int len = strlen(str_);
  this->str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str_[i] = str_[i];
  }
  this->str_[len] = '\0';
  this->capacity_ = len + 1;
  this->size_ = len;
}

vector<String> String::Split(const String& str_div) {
  vector<String> ret;
  String temp = "";
  if (str_div.Size() > Size()) {
    return {temp};
  }
  for (size_t i = 0; i < Size() - str_div.Size(); ++i) {
    String temp1;
    for (size_t j = i; j < i + str_div.Size(); ++j) {
      temp1 += (*this)[j];
    }
    if (i < Size() - str_div.Size() && temp1 != str_div) {
      temp += (*this)[i];
    } else if (temp1 == str_div) {
      ret.push_back(temp);
      temp.Clear();
      i += str_div.Size() - 1;
    }
    if (i == Size() - str_div.Size() && temp1 != str_div) {
      temp += temp1;
      ret.push_back(temp);
    }
  }
  String temp2 = temp = "";
  for (size_t i = Size() - str_div.Size(); i < Size(); ++i) {
    temp += (*this)[i];
  }
  temp == str_div ? ret.push_back(temp2) : temp.Clear();
  return ret;
}

String String::Join(const std::vector<String>& vec) const {
  String result = "";
  if (vec.empty()) {
    return result;
  }
  for (size_t i = 0; i < vec.size(); ++i) {
    result += vec[i];
    if (i < vec.size() - 1) {
      result += *this;
    }
  }
  return result;
}

String::~String() { delete[] this->str_; }

// int main() {
//   String s = "aba";
//   s.Resize(1);
//   cout << s.Size();
// }
