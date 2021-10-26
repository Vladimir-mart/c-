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
  char* str;
  size_t capacity = 0;
  size_t size = 0;

 public:
  String& operator=(const String& other);
  String& operator=(const char* other);
  String& operator+=(const String& other);
  String& operator+=(const char* other);
  String& operator+=(const char other);
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
  String(const size_t kSiz, const char& sim);
  void PushBack(const char elem);
  void Clear();
  void ShrinkToFit();
  void Swap(String& other);
  char& operator[](const size_t index) { return this->str[index]; }
  const char& operator[](const size_t index) const { return this->str[index]; }
  char& Front() { return this->str[0]; }
  const char& Front() const { return this->str[0]; }
  char& Back() { return this->str[size - 1]; }
  const char& Back() const { return this->str[size - 1]; }
  bool Empty() const { return size == 0; }
  bool operator<(const String& str) const;
  bool operator>(const String&) const;
  bool operator<=(const String&) const;
  bool operator>=(const String&) const;
  bool operator==(const String&) const;
  bool operator!=(const String&) const;
  friend std::ostream& operator<<(std::ostream&, const String&);
  friend std::istream& operator>>(std::istream&, String&);
  vector<String> Split(const String& str_div = " "); 
  String Join(const std::vector<String>& vec) const;
  ~String();
};

String operator*(String str, int m);
String operator+(const String& other1, const String& other2);
