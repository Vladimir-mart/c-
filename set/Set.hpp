#pragma once

#include <iostream>

template <typename Key, typename C = std::less<Key>>
class Set {
 public:
  typedef size_t SizeType;
  typedef Key KeyType;
  typedef KeyType ValueType;
  typedef C KeyCompare;
  typedef KeyCompare ValueCompare;
  Set() = default;
  Set(const Set&);
  Set& operator=(const Set&);
  ~Set();
  SizeType Size() const;
  bool Empty() const;
  void Clear();
  void Insert(const ValueType&);
  void Erase(const ValueType&);
  bool Find(const ValueType&) const;

 private:
  struct Node {
    Node(const Key&);
    Node* left = nullptr;
    Node* right = nullptr;
    ValueType val = 0;
    int priority = 0;
    SizeType amount = 1;
  };
  static int GetRandomNumber();
  Node* Build(ValueType*, SizeType);
  void Heapify(Node* root);
  bool Compare(const ValueType&, const ValueType&, const C& comp = C()) const;
  SizeType GetAmount(Node*) const;
  void UpdateAmount(Node*);
  void EraseRecursive(Node*&, const ValueType&);
  void ClearRecursive(Node*);
  Node* Merge(Node*, Node*);
  void Split(Node*, const ValueType&, Node*&, Node*&);
  bool FindRecursive(Node*, const ValueType&) const;
  Node* root_ = nullptr;
};
