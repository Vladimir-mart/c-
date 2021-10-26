#include "String.hpp"

using std::cin;
using std::cout;

void String::Swap(String& other) {
  char* temp = this->str;
  this->str = other.str;
  other.str = temp;
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

String& String::operator=(const char* kOther) {
  if (this->size > 0) {
    delete[] str;
  }
  int len = strlen(kOther);
  this->str = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str[i] = kOther[i];
  }
  this->str[len] = '\0';
  this->capacity = len + 1;
  this->size = len;
  return *this;
}

String& String::operator=(const String& other) {
  if (this->str == other.Data()) {
    return *this;
  }
  if (this->size > 0) {
    delete[] this->str;
  }
  int len = other.Size();
  this->str = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str[i] = other.str[i];
  }
  this->str[len] = '\0';
  this->capacity = len + 1;
  this->size = len;
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
  for (size_t i = 0; i < min(this->size, str.Size()); ++i) {
    if (this->str[i] < str[i]) {
      return this->str[i] < str[i];
    }
  }
  return (this->size < str.Size());
}

void String::ShrinkToFit() {
  if (size < capacity - 1) {
    char* strt = new char[this->size + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    delete[] this->str;
    this->str = strt;
    this->capacity = this->size + 1;
    this->str[this->size] = '\0';
  }
}

void String::Reserve(size_t new_cap) {
  if (this->capacity < new_cap) {
    char* strt = new char[new_cap * 2 + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    delete[] this->str;
    this->str = strt;
    this->capacity = 2 * new_cap + 1;
    this->str[this->size] = '\0';
  }
}

void String::Resize(size_t new_size, char elem) {
  if (new_size >= this->capacity) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    for (size_t j = 0; j < new_size - this->size; j++, i++) {
      strt[i] = elem;
    }
    if (this->size > 0) {
      delete[] this->str;
    }
    this->str = strt;
    this->size = new_size;
    this->capacity = new_size + 1;
    this->str[this->size] = '\0';
  } else {
    this->size = new_size;
    this->str[new_size] = '\0';
  }
}

void String::Resize(size_t new_size) {
  if (new_size >= this->capacity) {
    char* strt = new char[new_size + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    if (this->size > 0) {
      delete[] this->str;
    }
    this->str = strt;
    this->size = new_size;
    this->capacity = new_size + 1;
    this->str[this->size] = '\0';
  } else {
    this->size = new_size;
    this->str[new_size] = '\0';
  }
}

// String& String::operator*=(int m) {
//     // cout <<  " ========== " ;
//     String t = this->str;
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
//     // if (this->str != nullptr) {
//     //     delete[] this->str;
//     // }
//      this->str = t.str;
//     // return *this;
// }

String& String::operator*=(int m) {
  (*this) = (*this) * m;
  return *this;
}

String operator*(String str, int m) {
  String result = "";
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

  if (this->capacity <= (this->size + len + 1)) {
    char* strt = new char[this->size + len + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    for (size_t j = 0; j < len; j++, i++) {
      strt[i] = other[j];
    }
    if (this->size > 0) {
      delete[] this->str;
    }
    this->str = strt;
    this->size = len + this->size;
    this->capacity = this->size + 1;
    this->str[this->size] = '\0';
  } else {
    for (size_t i = this->size, j = 0; j < len; ++i, ++j) {
      this->str[i] = other[j];
    }
    this->size += len;
    this->str[this->size + 1] = '\0';
  }
  return *this;
}

String& String::operator+=(const String& other) {
  size_t len = other.Size();

  if (this->capacity <= (this->size + len + 1)) {
    char* strt = new char[this->size + len + 1];
    size_t i = 0;
    for (; i < this->size; ++i) {
      strt[i] = this->str[i];
    }
    for (size_t j = 0; j < len; j++, i++) {
      strt[i] = other.str[j];
    }
    delete[] this->str;
    this->str = strt;
    this->size = len + this->size;
    this->capacity = this->size + 1;
    this->str[this->size] = '\0';
  } else {
    for (size_t i = this->size, j = 0; j < len; ++i, ++j) {
      this->str[i] = other.str[j];
    }
    this->size += len;
    this->str[this->size + 1] = '\0';
  }
  return *this;
}

String operator+(const String& other1, const String& other2) {
  String result = other1;
  result += other2;
  return result;
}

void String::PopBack() {
  if (this->size == 0) {
    return;
  }
  --size;
}

size_t String::Capacity() const { return this->capacity - 1; }

void String::PushBack(const char kElem) {
  if (this->size == (this->capacity - 1)) {
    int len = this->size * 2;
    char* strt = new char[len + 1];
    for (size_t i = 0; i < size; ++i) {
      strt[i] = str[i];
    }
    delete[] this->str;
    this->str = strt;
    this->str[size] = kElem;
    this->size = size + 1;
    this->capacity = len + 1;
    this->str[size] = '\0';
  } else {
    this->str[size] = kElem;
    this->size = size + 1;
    this->str[size] = '\0';
  }
}

size_t String::Size() const { return this->size; }

void String::Clear() { this->size = 0; }

const char* String::Data() const {
  this->str[this->size] = '\0';
  return str;
}

String::String(const size_t kSiz, const char& sim) {
  this->str = new char[kSiz + 1];
  for (size_t i = 0; i < kSiz; ++i) {
    str[i] = sim;
  }
  this->str[kSiz] = '\0';
  this->capacity = kSiz + 1;
  this->size = kSiz;
}

String::String() {
  this->str = nullptr;
  this->capacity = 1;
}

String::String(const String& str) {
  size_t len = str.Size();
  this->str = new char[len + 1];
  for (size_t i = 0; i < len; ++i) {
    this->str[i] = str.str[i];
  }
  this->str[len] = '\0';
  this->capacity = len + 1;
  this->size = len;
}

String::String(const char* str) {
  int len = strlen(str);
  this->str = new char[len + 1];
  for (int i = 0; i < len; ++i) {
    this->str[i] = str[i];
  }
  this->str[len] = '\0';
  this->capacity = len + 1;
  this->size = len;
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

String::~String() { delete[] this->str; }

// int main() {
//   String s = "aba";
//   s.Resize(1);
//   cout << s.Size();
// }
