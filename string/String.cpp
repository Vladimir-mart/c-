#include "String.hpp"

String::String(const size_t kSiz, const char &symbol) {
  this->str_ = new char[kSiz + 1];
  for (size_t i = 0; i < kSiz; ++i) {
    this->str_[i] = symbol;
  }
  this->capacity_ = kSiz + 1;
  this->size_ = kSiz;
}

String::String() {
  this->str_ = nullptr;
  this->capacity_ = 1;
}

String::String(const String &str) {
  size_t len = str.Size();
  this->str_ = new char[len + 1];
  for (size_t i = 0; i < len; ++i) {
    this->str_[i] = str.str_[i];
  }
  this->capacity_ = len + 1;
  this->size_ = len;
}

String::String(const char* str) {
  int len = strlen(str);
  this->str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str_[i] = str[i];
  }
  this->capacity_ = len + 1;
  this->size_ = len;
}

String::String(char symbol) {
  this->str_ = new char[2];
  this->str_[0] = symbol;
  this->size_ = 1;
  this->capacity_ = 2;
}

void String::Swap(String &other) {
  char* temp = this->str_;
  this->str_ = other.str_;
  other.str_ = temp;
}

std::istream &operator>>(std::istream &in, String &str) {
  str.Clear();
  char ch;
  while (in >> ch) {
    str.PushBack(ch);
  }
  return in;
}

std::ostream &operator<<(std::ostream &out, const String &str) {
  for (size_t i = 0; i < str.Size(); ++i) {
    out << str[i];
  }
  return out;
}

String &String::operator=(const String &other) {
  // Data
  if (this->str_ == other.str_) {
    return *this;
  }
  if (this != &other) {
    delete[] this->str_;
  }
  this->str_ = new char[other.Size() + 1];
  for (size_t i = 0; i < other.Size(); ++i) {
    this->str_[i] = other.str_[i];
  }
  this->capacity_ = other.Size() + 1;
  this->size_ = other.Size();
  return *this;
}

bool String::operator<=(const String &str) const { return !(*this > str); }

bool String::operator>=(const String &str) const { return !(*this < str); }

bool String::operator==(const String &str) const {
  return !(*this < str || *this > str);
}

bool String::operator!=(const String &str) const { return !(*this == str); }

bool operator>(const String &str2, const String &str1) { return str1 < str2; }

bool operator<(const String &str2, const String &str1) {
  for (size_t i = 0; i < min(str2.Size(), str1.Size()); ++i) {
    if (str2[i] < str1[i]) {
      return str2[i] < str1[i];
    }
  }
  return (str2.Size() < str1.Size());
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
  }
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size >= this->capacity_) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < this->size_; ++i) {
      strt[i] = this->str_[i];
    }
    for (size_t j = 0; j < new_size - this->size_; j++, i++) {
      strt[i] = symbol;
    }
    if (this->size_ > 0) {
      delete[] this->str_;
    }
    this->str_ = strt;
    this->size_ = new_size;
    this->capacity_ = new_size + 1;
  } else {
    this->size_ = new_size;
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
  } else {
    this->size_ = new_size;
  }
}

String &String::operator*=(int m) {
  (*this) = (*this) * m;
  return *this;
}

String operator*(String str, unsigned int m) {
  String result;
  if (m == 0) {
    return result;
  }
  if (m == 1) {
    result = str;
  } else {
    result = str * (m / 2);
    result += result;
    if (m % 2 == 1) {
      result += str;
    }
  }
  return result;
}

String &String::operator+=(const String &other) {
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
  } else {
    for (size_t i = this->size_, j = 0; j < len; ++i, ++j) {
      this->str_[i] = other.str_[j];
    }
    this->size_ += len;
  }
  return *this;
}

String operator+(const String &other1, const String &other2) {
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

void String::PushBack(const char kSymbol) {
  if (this->size_ >= (this->capacity_ - 1)) {
    char* strt = nullptr;
    if (this->size_ == 0) {
      if (capacity_ == 1) {
        strt = new char[capacity_ * 2 + 1];
        capacity_ *= 2 + 1;
      } else {
        strt = new char[(capacity_ - 1) * 2 + 1];
        capacity_ = (capacity_ - 1) * 2 + 1;
      }
    } else {
      strt = new char[(capacity_ - 1) * 2 + 1];
      capacity_ = (capacity_ - 1) * 2 + 1;
    }
    for (size_t i = 0; i < size_; ++i) {
      strt[i] = this->str_[i];
    }
    delete[] this->str_;
    this->str_ = strt;
    this->str_[size_] = kSymbol;
    ++size_;
  } else {
    this->str_[size_] = kSymbol;
    this->size_ += 1;
  }
}

size_t String::Size() const { return this->size_; }

void String::Clear() { this->size_ = 0; }

const char* String::Data() const {
  if (this->size_ > 0) {
    this->str_[this->size_] = '\0';
  }
  return this->str_;
}

String String::Join(const std::vector<String> &vec) const {
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

String::~String() { delete[] str_; }

void String::SplitHelpSize(String &temp, const String &str_div,
                           size_t &str_point, String &t, vector<String> &ret) {
  int pol = 0;
  int iter = 0;
  for (int iq = 0; iq < Size(); ++iq) {
    if (t == str_div) {
      for (size_t i = 0; i < str_div.Size(); ++i) {
        temp.PopBack();
      }
      ret.push_back(temp);
      temp.Clear();
      t.Clear();
      if (str_point < this->size_) {
        for (size_t i = 0; i < str_div.Size(); ++i) {
          t += this->str_[str_point];
          ++str_point;
        }
      }
      temp = t;
      ++pol;
      ++iter;
    }
    // проверка, что нет str_div, которые идут подряд, если - да,
    // то удаляем все в одном цикле, если нет, то выходим
    if (iter == 2 && pol <= 1) {
      break;
    } else {
      pol = 0;
      iter = 0;
    }
  }
}

vector<String> String::Split(const String &str_div) {
  vector<String> ret;
  String str_push;
  String div_temp;
  if (this->size_ == 0) {
    ret.push_back("");
    return ret;
  }
  for (size_t i = 0; i < str_div.Size(); i++) {
    div_temp += this->str_[i];
  }
  str_push = div_temp;
  size_t str_point = str_push.Size();
  for (; str_point <= Size();) {
    SplitHelpSize(str_push, str_div, str_point, div_temp, ret);
    str_push.PushBack(str_[str_point]);
    ++str_point;
    for (size_t k = str_push.Size() - str_div.Size(), i = 0; k < str_push.Size();
         ++k, ++i) {
      div_temp[i] = str_push[k];
    }
  }
  str_push.PopBack();
  ret.push_back(str_push);
  str_push.Clear();
  div_temp.Clear();
  return ret;
}

