#include "Set.hpp"

#include <random>
#include <iostream>
#include <stack>

using std::stack;

template <typename Key, typename C>
int Set<Key, C>::GetRandomNumber() {
  std::mt19937 gen;
  std::uniform_int_distribution<> dis(-1000000, 1000000);
  return dis(gen);
}

template <typename Key, typename C>
Set<Key, C>::Node::Node(const Key& elem): left(nullptr), right(nullptr), val(elem), priority(GetRandomNumber()) {}

template <typename Key, typename C>
size_t Set<Key, C>::GetAmount(Node* root) const{
  return root ? root->amount : 0;
}

template <typename Key, typename C>
void Set<Key, C>::UpdateAmount(Node* root) {
  if (root) {
    root->amount = 1 + GetAmount(root->left) + GetAmount(root->right);
  }
}

template <typename Key, typename C>
void Set<Key, C>::Heapify(Node* root) {
  if (root == nullptr) {
    return;
  }
  Node* max = root;
  if (root->left && root->left->priority > max->priority) {
    max = root->left;
  }
  if (root->right && root->right->priority > max->priority) {
    max = root->right;
  }
  if (root != max) {
    std::swap(root->priority, max->priority);
    Heapify(max);
  }
}
template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::Build(Key* arr, size_t sz) {
  if (sz == 0) {
    return nullptr;
  }
  size_t mid = sz / 2;
  Node* new_root = new Node(arr[mid]);
  new_root->left = Build(arr, mid);
  new_root->right = Build(arr + mid + 1, sz - mid - 1);
  Heapify(new_root);
  UpdateAmount(new_root);
  return new_root;
}

template <typename Key, typename C>
Set<Key, C>::Set(const Set& st) {
  Key* arr = new Key[st.Size()];
  size_t counter = 0;
  stack<Node*> s;
  Node* temp = st.root_;
  while (temp || !s.empty()) {
    while (temp) {
      s.push(temp);
      temp = temp->left;
    }
    Node* popped = s.top();
    s.pop();
    arr[counter] = popped->val;
    ++counter;
    temp = popped->right;
  }
  root_ = Build(arr, st.Size());
  delete[] arr;
}

template <typename Key, typename C>
Set<Key, C>& Set<Key, C>::operator=(const Set& st) {
  if (root_ == st.root_) {
    return *this;
  }
  ClearRecursive(root_);
  Key* arr = new Key[st.Size()];
  size_t counter = 0;
  stack<Node*> s;
  Node* temp = st.root_;
  while (temp || !s.empty()) {
    while (temp) {
      s.push(temp);
      temp = temp->left;
    }
    Node* popped = s.top();
    s.pop();
    arr[counter] = popped->val;
    ++counter;
    temp = popped->right;
  }
  root_ = Build(arr, st.Size());
  delete[] arr;
  return *this;
}

template <typename Key, typename C>
size_t Set<Key, C>::Size() const {
  return GetAmount(root_);
}

template <typename Key, typename C>
void Set<Key, C>::ClearRecursive(Node* root) {
  if (root) {
    ClearRecursive(root->left);
    ClearRecursive(root->right);
    delete root;
  }
}

template <typename Key, typename C>
void Set<Key, C>::Clear() {
  ClearRecursive(root_);
  root_ = nullptr;
}

template <typename Key, typename C>
Set<Key, C>::~Set() {
  ClearRecursive(root_);
}

template <typename Key, typename C>
bool Set<Key, C>::Compare(const Key& elem1, const Key& elem2, const C& comp) const {
  return comp(elem1, elem2);
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::Merge(Node* root1, Node* root2) {
  if (root1 == nullptr || root2 == nullptr) {
    return root1 ? root1 : root2;
  }
  if (root1->priority > root2->priority) {
    root1->right = Merge(root1->right, root2);
    UpdateAmount(root1);
    return root1;
  }
  root2->left = Merge(root1, root2->left);
  UpdateAmount(root2);
  return root2;
}

template <typename Key, typename C>
void Set<Key, C>::Split(Node* root, const Key& elem, Node*& left, Node*& right) {
  if (root == nullptr) {
    left = right = nullptr;
  } else if (Compare(root->val, elem)) {
    Split(root->right, elem, root->right, right);
    left = root;
  } else {
    Split(root->left, elem, left, root->left);
    right = root;
  }
  UpdateAmount(left);
  UpdateAmount(right);
}

template <typename Key, typename C>
void Set<Key, C>::Insert(const Key& elem) {
  if (FindRecursive(root_, elem)) {
    return;
  }
  Node* new_node = new Node(elem);
  Node* temp1 = nullptr;
  Node* temp2 = nullptr;
  Split(root_, elem, temp1, temp2);
  root_ = Merge(temp1, new_node);
  root_ = Merge(root_, temp2);
}

template <typename Key, typename C>
bool Set<Key, C>::Empty() const { return GetAmount(root_) == 0; }

template <typename Key, typename C>
bool Set<Key, C>::FindRecursive(Node* root, const Key& elem) const {
  if (root == nullptr || root->val == elem) {
    return root ? root->val == elem : false;
  }
  if (root->val > elem) {
    return FindRecursive(root->left, elem);
  }
  return FindRecursive(root->right, elem);
}

template <typename Key, typename C>
bool Set<Key, C>::Find(const Key& elem) const {
  return FindRecursive(root_, elem);
}

template <typename Key, typename C>
void Set<Key, C>::Erase(const Key& elem) {
  Node* node1 = nullptr;
  Node* node2 = nullptr;
  Node* node3 = nullptr;
  Split(root_, elem, node1, node2);
  Split(node2, elem + 1, node2, node3);
  root_ = Merge(node1, node3);
  UpdateAmount(root_);
  ClearRecursive(node2);
}
