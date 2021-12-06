#pragma once

#include <iostream>
#include <set>

template <typename Key, typename C = std::less<Key>>
class Set {
 public:
  // NO LINT
  typedef size_t size_type;
  typedef Key key_type;
  typedef key_type value_type;
  typedef C key_compare;
  typedef key_compare ValueCompare;
  Set() = default;
  Set(const Set&);
  Set& operator=(const Set&);
  ~Set();
  size_type Size() const;
  bool Empty() const;
  void Clear();
  void Insert(const value_type&);
  void Erase(const value_type&);
  bool Find(const value_type&) const;

 private:
  struct Node {
    Node(const Key&);
    Node* left = nullptr;
    Node* right = nullptr;
    value_type val = 0;
    int priority = 0;
    size_type amount = 1;
  };
  static int GetRandomNumber();
  Node* Build(value_type*, size_type);
  void Heapify(Node* root);
  bool Compare(const value_type&, const value_type&, const C& comp = C()) const;
  size_type GetAmount(Node*) const;
  void UpdateAmount(Node*);
  void EraseRecursive(Node*&, const value_type&);
  void ClearRecursive(Node*);
  Node* Merge(Node*, Node*);
  void Split(Node*, const value_type&, Node*&, Node*&);
  bool FindRecursive(Node*, const value_type&) const;
  Node* root_ = nullptr;
};
