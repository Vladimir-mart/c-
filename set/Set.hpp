#pragma once

#include <functional>
#include <iostream>
#include <random>
#include <stack>

using std::stack;

template <typename Key, typename C = std::less<Key>>
class Set {
 public:
  typedef size_t size_type;           // NOLINT
  typedef Key key_type;               // NOLINT
  typedef key_type value_type;        // NOLINT
  typedef C key_compare;              // NOLINT
  typedef key_compare value_compare;  // NOLINT
  Set() = default;
  Set(const Set&);
  Set& operator=(const Set&);
  ~Set();
  size_type Size() const;
  bool Empty() const;
  void Clear();
  void Insert(const Key&);
  void Erase(const Key&);
  bool Find(const Key&) const;

 private:
  struct Node {
    Node(const Key&);
    Node* left = nullptr;
    Node* right = nullptr;
    value_type val = 0;
    int priority = 0;
    size_t amount = 1;
  };
  static int GetRandomNumber();
  Node* Build(Key*, size_t);
  void Heapify(Node* root);
  bool Compare(const Key&, const Key&, const C& comp = C()) const;
  size_t GetAmount(Node*) const;
  void UpdateAmount(Node*);
  void EraseRecursive(Node*&, const Key&);
  void ClearRecursive(Node*);
  Node* Merge(Node*, Node*);
  void Split(Node*, const Key&, Node*&, Node*&);
  bool FindRecursive(Node*, const Key&) const;
  Node* root_ = nullptr;
};
