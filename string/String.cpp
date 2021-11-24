#include "String.hpp"

String::String(const size_t kSiz, const char& symbol) {
  str_ = new char[kSiz + 1];
  for (size_t i = 0; i < kSiz; ++i) {
    str_[i] = symbol;
  }
  capacity_ = kSiz + 1;
  size_ = kSiz;
}

String::String() {
  str_ = nullptr;
  capacity_ = 1;
}

String::String(const String& str) {
  size_t len = str.Size();
  str_ = new char[len + 1];
  for (size_t i = 0; i < len; ++i) {
    str_[i] = str.str_[i];
  }
  capacity_ = len + 1;
  size_ = len;
}

String::String(const char* str) {
  int len = strlen(str);
  str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    str_[i] = str[i];
  }
  capacity_ = len + 1;
  size_ = len;
}

String::String(char symbol) {
  str_ = new char[2];
  str_[0] = symbol;
  size_ = 1;
  capacity_ = 2;
}

void String::Swap(String& other) {
  char* temp = str_;
  str_ = other.str_;
  other.str_ = temp;
}

std::istream& operator>>(std::istream& in, String& str) {
  str.Clear();
  char ch;
  while (in >> ch) {
    str.PushBack(ch);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
  for (size_t i = 0; i < str.Size(); ++i) {
    out << str[i];
  }
  return out;
}

String& String::operator=(const String& other) {
  // Data
  if (str_ == other.str_) {
    return *this;
  }
  if (this != &other) {
    delete[] str_;
  }
  str_ = new char[other.Size() + 1];
  for (size_t i = 0; i < other.Size(); ++i) {
    str_[i] = other.str_[i];
  }
  capacity_ = other.Size() + 1;
  size_ = other.Size();
  return *this;
}

bool String::operator<=(const String& str) const { return !(*this > str); }

bool String::operator>=(const String& str) const { return !(*this < str); }

bool String::operator==(const String& str) const {
  return !(*this < str || *this > str);
}

bool String::operator!=(const String& str) const { return !(*this == str); }

bool operator>(const String& str2, const String& str1) { return str1 < str2; }

bool operator<(const String& str2, const String& str1) {
  for (size_t i = 0; i < min(str2.Size(), str1.Size()); ++i) {
    if (str2[i] < str1[i]) {
      return str2[i] < str1[i];
    }
  }
  return (str2.Size() < str1.Size());
}

void String::ShrinkToFit() {
  if (size_ < capacity_ - 1) {
    char* strt = new char[size_ + 1];
    size_t i = 0;
    for (; i < size_; ++i) {
      strt[i] = str_[i];
    }
    delete[] str_;
    str_ = strt;
    capacity_ = size_ + 1;
  }
}

void String::Reserve(size_t new_cap) {
  if (capacity_ < new_cap) {
    char* strt = new char[new_cap * 2 + 1];
    size_t i = 0;
    for (; i < size_; ++i) {
      strt[i] = str_[i];
    }
    delete[] str_;
    str_ = strt;
    capacity_ = 2 * new_cap + 1;
  }
}

void String::Resize(size_t new_size, char symbol) {
  pol_resize_ = 1;
  pol_res_s_ = symbol;
  Resize(new_size);
  pol_resize_ = 0;
}

void String::Resize(size_t new_size) {
  if (new_size >= capacity_) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < size_; ++i) {
      strt[i] = str_[i];
    }
    if (pol_resize_ == 1) {
      for (size_t j = 0; j < new_size - size_; j++, i++) {
        strt[i] = pol_res_s_;
      }
    }
    if (size_ > 0) {
      delete[] str_;
    }
    str_ = strt;
    size_ = new_size;
    capacity_ = new_size + 1;
  } else {
    size_ = new_size;
  }
}

String& String::operator*=(int m) {
  *this = *this * m;
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

String& String::operator+=(const String& other) {
  size_t len = other.Size();
  if (capacity_ <= (size_ + len + 1)) {
    char* strt = new char[size_ + len + 1];
    size_t i = 0;
    for (; i < size_; ++i) {
      strt[i] = str_[i];
    }
    for (size_t j = 0; j < len; j++, i++) {
      strt[i] = other.str_[j];
    }
    delete[] str_;
    str_ = strt;
    size_ = len + size_;
    capacity_ = size_ + 1;
  } else {
    for (size_t i = size_, j = 0; j < len; ++i, ++j) {
      str_[i] = other.str_[j];
    }
    size_ += len;
  }
  return *this;
}

String operator+(const String& other1, const String& other2) {
  String result = other1;
  result += other2;
  return result;
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  --size_;
}

size_t String::Capacity() const { return capacity_ - 1; }

void String::PushBack(const char kSymbol) {
  if (size_ >= (capacity_ - 1) && size_ > 1) {
    char* strt = nullptr;
    strt = new char[size_ * 2 + 1];
    capacity_ = size_ * 2 + 1;
    for (size_t i = 0; i < size_; ++i) {
      strt[i] = str_[i];
    }
    delete[] str_;
    str_ = strt;
    str_[size_] = kSymbol;
    ++size_;
  } else {
    if (size_ == 0) {
      delete str_;
      if (capacity_ == 1) {
        str_ = new char[2];
        capacity_ = 2;
      } else {
        str_ = new char[(capacity_ - 1) * 2 + 1];
        capacity_ = (capacity_ - 1) * 2 + 1;
      }
      str_[0] = kSymbol;
    } else {
      str_[size_] = kSymbol;
    }
    ++size_;
  }
}

size_t String::Size() const { return size_; }

void String::Clear() { size_ = 0; }

const char* String::Data() const {
  if (size_ > 0) {
    str_[size_] = '\0';
  }
  return str_;
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

String::~String() { delete[] str_; }

void String::SplitHelpSize(String& temp, const String& str_div,
                           size_t& str_point, String& t, vector<String>& ret) {
  int pol = 0;
  int iter = 0;
  for (size_t iq = 0; iq < Size(); ++iq) {
    if (t == str_div) {
      for (size_t i = 0; i < str_div.Size(); ++i) {
        temp.PopBack();
      }
      ret.push_back(temp);
      temp.Clear();
      t.Clear();
      if (str_point < size_) {
        for (size_t i = 0; i < str_div.Size(); ++i) {
          t += str_[str_point];
          ++str_point;
        }
      }
      temp = t;
      ++pol;
      ++iter;
    }
    if (iter == 2 && pol <= 1) {
      break;
    }
    if (iter == 2 && pol <= 1) {
      pol = 0;
      iter = 0;
    }
  }
}

vector<String> String::Split(const String& str_div) {
  vector<String> ret;
  String str_push;
  String div_temp;
  if (size_ == 0) {
    ret.push_back("");
    return ret;
  }
  for (size_t i = 0; i < str_div.Size(); i++) {
    div_temp += str_[i];
  }
  str_push = div_temp;
  size_t str_point = str_push.Size();
  for (; str_point <= Size();) {
    SplitHelpSize(str_push, str_div, str_point, div_temp, ret);
    if (str_point < Size()) {
      str_push.PushBack(str_[str_point]);
    }
    ++str_point;
    for (size_t k = str_push.Size() - str_div.Size(), i = 0;
         k < str_push.Size(); ++k, ++i) {
      div_temp[i] = str_push[k];
    }
  }
  ret.push_back(str_push);
  str_push.Clear();
  div_temp.Clear();
  return ret;
}
 
