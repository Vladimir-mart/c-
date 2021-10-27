#pragma once

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

using std::cout;
using std::min;
using std::size_t;
using std::strlen;
using std::string;
using std::vector;

class String {
 private:
  char* str_;
  size_t capacity_ = 0;
  size_t size_ = 0;

 public:
  String& operator=(const String&);
  String& operator=(const char*);
  String& operator+=(const String&);
  String& operator+=(const char*);
  String& operator+=(char kOther);
  String& operator*=(int);
  void Resize(size_t new_size);
  void Resize(size_t new_size, char elem);
  String();
  const char* Data() const;
  String(const String&);
  size_t Capacity() const;
  size_t Size() const;
  void Reserve(size_t new_cap);
  String(const char* str);
  void PopBack();
  String(size_t kSiz, const char& sim);
  void PushBack(char);
  void Clear();
  void ShrinkToFit();
  void Swap(String& other);
  char& operator[](const size_t kIndex) { return this->str_[kIndex]; }
  const char& operator[](const size_t kIndex) const { return this->str_[kIndex]; }
  char& Front() { return this->str_[0]; }
  const char& Front() const { return this->str_[0]; }
  char& Back() { return this->str_[size_ - 1]; }
  const char& Back() const { return this->str_[size_ - 1]; }
  bool Empty() const { return size_ == 0; }
  bool operator<(const String& str) const;
  bool operator>(const String&) const;
  bool operator<=(const String&) const;
  bool operator>=(const String&) const;
  bool operator==(const String&) const;
  bool operator!=(const String&) const;
  void SplitHelpSize(String&, const String&, size_t&, String&, vector<String>&);
  friend std::ostream& operator<<(std::ostream&, const String&);
  friend std::istream& operator>>(std::istream&, String&);
  vector<String> Split(const String& str_div = " "); 
  String Join(const std::vector<String>& vec) const;
  ~String();
};


String operator*(String str, int m);
String operator+(const String& other1, const String& other2);


