#include "String.hpp"

using std::cin;
using std::cout;

void String::Swap(String& other) {
  char* temp = this->str_;
  this->str_ = other.str_;
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

String& String::operator=(const char* k_other) {
  if (this->size_ > 0) {
    delete[] this->str_;
  }
  int len = strlen(k_other);
  this->str_ = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str_[i] = k_other[i];
  }
  this->capacity_ = len + 1;
  this->size_ = len;
  return *this;
}

String& String::operator=(const String& other) {
  // Data
  if (this->str_ == other.Data()) {
    return *this;
  }
  if (this->size_ > 0) {
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

bool String::operator>(const String& str) const { return str < *this; }

bool String::operator<=(const String& str) const { return !(*this > str); }

bool String::operator>=(const String& str) const { return !(*this < str); }

bool String::operator==(const String& str) const {
  return !(*this < str || *this > str);
}

bool String::operator!=(const String& str) const { return !(*this == str); }

bool String::operator<(const String& str) const {
  for (size_t i = 0; i < min(this->size_, str.Size()); ++i) {
    if (this->str_[i] < str[i]) {
      return this->str_[i] < str[i];
    }
  }
  return (this->size_ < str.Size());
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

String& String::operator*=(int m) {
  (*this) = (*this) * m;
  return *this;
}

String operator*(String str, int m) {
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
  } else {
    for (size_t i = this->size_, j = 0; j < len; ++i, ++j) {
      this->str_[i] = other[j];
    }
    this->size_ += len;
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
  } else {
    for (size_t i = this->size_, j = 0; j < len; ++i, ++j) {
      this->str_[i] = other.str_[j];
    }
    this->size_ += len;
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
    this->str_[size_] = kElem;
    ++size_;
  } else {
    this->str_[size_] = kElem;
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

String::String(const size_t kSiz, const char& sim) {
  this->str_ = new char[kSiz + 1];
  for (size_t i = 0; i < kSiz; ++i) {
    this->str_[i] = sim;
  }
  this->capacity_ = kSiz + 1;
  this->size_ = kSiz;
}

String::String() {
  this->str_ = nullptr;
  this->capacity_ = 1;
}

String::String(const String& str) {
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

vector<String> String::Split(const String& str_div) {
  std::vector<String> result;
  String temp;
  if (Size() < str_div.Size()) {
    return {temp};
  }
  for (size_t i = 0; i <= Size() - str_div.Size(); ++i) {
    String temp1;
    for (size_t j = i; j < i + str_div.Size(); ++j) {
      temp1 += (*this)[j];
    }
    if (i < Size() - str_div.Size() && temp1 != str_div) {
      temp += (*this)[i];
    } else if (temp1 == str_div) {
      result.push_back(temp);
      temp.Clear();
      i += str_div.Size() - 1;
    }
    if (i == Size() - str_div.Size() && temp1 != str_div) {
      temp += temp1;
      result.push_back(temp);
    }
  }
  String temp2 = temp;
  for (size_t i = Size() - str_div.Size(); i < Size(); ++i) {
    temp += (*this)[i];
  }
  temp == str_div ? result.push_back(temp2) : temp.Clear();
  return result;
}

// int main() {
//   // String s = "";
//   // char ch = 'a';
//   // while (ch != '9'){
//   //   cin >> ch;
//   //   s.PushBack(ch);
//   //   cout << s << "\n";
//   // }
//     std::vector<String> expected{"hello", "world,no split here", "", "1",
//     ""}; if(expected == String("hello, world,no split here, , 1, ").Split(",
//     "));

// }
