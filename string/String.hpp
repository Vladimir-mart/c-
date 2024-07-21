#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

using std::min;
using std::size_t;
using std::strlen;
using std::vector;

class String {
 public:
  String(const char* str);
  String(const String&);
  String();
  String(char);
  String(size_t, const char& symbol);
  ~String();

  String& operator=(const String&);
  String& operator+=(const String&);
  String& operator*=(int);

  void Resize(size_t new_size);
  void Resize(size_t new_size, char symbol);
  size_t Capacity() const;
  size_t Size() const;
  void Reserve(size_t new_cap);
  void Clear();
  void ShrinkToFit();
  const char* Data() const;

  void PopBack();
  void PushBack(char);
  void Swap(String& other);

  char& operator[](const size_t kIndex) { return this->str_[kIndex]; }
  const char& operator[](const size_t kIndex) const {
    return this->str_[kIndex];
  }

  char& Front() { return this->str_[0]; }
  const char& Front() const { return this->str_[0]; }
  char& Back() { return this->str_[size_ - 1]; }
  const char& Back() const { return this->str_[size_ - 1]; }
  bool Empty() const { return size_ == 0; }

  bool operator==(const String&) const;
  bool operator!=(const String&) const;

  friend std::ostream& operator<<(std::ostream&, const String&);
  friend std::istream& operator>>(std::istream&, String&);

  void SplitHelpSize(String&, const String&, size_t&, String&, vector<String>&);
  vector<String> Split(const String& str_div = " ");
  String Join(const std::vector<String>& vec) const;

 private:
  char* str_ = nullptr;
  size_t capacity_ = 0;
  size_t size_ = 0;
  int pol_resize_ = 0;
  char pol_res_s_ = 0;
};

bool operator<=(const String&, const String&);
bool operator>=(const String&, const String&);
bool operator<(const String&, const String&);
bool operator>(const String&, const String&);

String operator*(String str, unsigned int m);
String operator+(const String& other1, const String& other2);
