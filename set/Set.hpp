#pragma once

#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <stack>

template <typename Key, typename C = std::less<Key>>
class Set {
public:
  typedef size_t size_type;           // NOLINT
  typedef Key key_type;               // NOLINT
  typedef key_type value_type;        // NOLINT
  typedef C key_compare;              // NOLINT
  typedef key_compare value_compare;  // NOLINT
  Set() = default;
  Set(const Set& st);
  Set& operator=(const Set& st);
  ~Set();
  bool Empty() const;
  void Insert(const Key& elem);
  void Clear();
  void Erase(const Key& elem);
  bool IsFind(const Key& val);
  size_t Size();
  void Inorder();
  
private:
  struct Node {
    Node(const Key& elem);
    Node* left = nullptr;
    Node* right = nullptr;
    size_t height;
    Key val;
  };
  Node* root_ = nullptr;
  size_t size_ = 0;
  bool increase_ = false;
  bool decrease_ = false;
  bool Compare(const Key& elem1, const Key& elem2, const C& comp = C()) const;
  void InorderRecursive(Node* root);
  Node* FindMin(Node* root);
  Node* RemoveMin(Node* root);
  Node* EraseRecursive(Node* root, const Key& elem);
  void ClearRecursive(Node* root);
  Node* InsertRecursive(Node* root, const Key& val);
  size_t HeightDefine(Node* node);
  int BalanceFactor(Node* node);
  void HeightFix(Node* node);
  Node* Balance(Node* node);
  Node* LeftRotation(Node* node);
  Node* RightRotation(Node* node);
  bool FindElem(Node* node, const Key& val);
  
public:
  class ConstIterator {
  public:
    typedef value_type value_type;                              // NOLINT
    typedef const Key& reference;                               // NOLINT
    typedef std::bidirectional_iterator_tag iterator_category;  // NOLINT
    typedef std::ptrdiff_t difference_type;                     // NOLINT
    typedef Key* pointer;                                       // NOLINT
    ConstIterator() = default;
    ConstIterator(const std::stack<Node*>& s, bool check);
    ConstIterator& operator++();
    ConstIterator& operator++(int);
    ConstIterator& operator--();
    ConstIterator& operator--(int);
    bool operator==(const ConstIterator& iter) const;
    bool operator!=(const ConstIterator& iter) const;
    const Key& operator*();
    
  private:
    std::stack<struct Node*> st_;
    struct Node* current_ = nullptr;
    struct Node* next_ = nullptr;
    struct Node* intermediate_ = nullptr;
  };
  
  template <typename Iterator>
  class CommonReverseIterator {
  public:
    CommonReverseIterator<Iterator>& operator++();
    CommonReverseIterator<Iterator>& operator++(int);
    CommonReverseIterator<Iterator>& operator--();
    CommonReverseIterator<Iterator>& operator--(int);
    bool operator==(const CommonReverseIterator<Iterator>& iter_) const;
    bool operator!=(const CommonReverseIterator<Iterator>& iter_) const;
    const Key& operator*();
    Iterator Base() const;
    
  private:
    Iterator iter_;
  };
  typedef ConstIterator const_iterator;                      // NOLINT
  typedef const_iterator iterator;                           // NOLINT
  typedef std::reverse_iterator<iterator> reverse_iterator;  // NOLINT
  typedef std::reverse_iterator<const_iterator>
  const_reverse_iterator;  // NOLINT
  iterator begin();                   // NOLINT
  const_iterator cbegin();            // NOLINT
  iterator end();                     // NOLINT
  const_iterator cend();              // NOLINT
  reverse_iterator rbegin();          // NOLINT
  reverse_iterator rend();            // NOLINT
  const_reverse_iterator crbegin();   // NOLINT
  const_reverse_iterator crend();     // NOLINT
  bool operator<(Set& st);
};

template <typename Key, typename C>
Set<Key, C>::Node::Node(const Key& elem)
: left(nullptr), right(nullptr), val(elem), height(1) {}

template <typename Key, typename C>
bool Set<Key, C>::Compare(const Key& elem1, const Key& elem2,
                          const C& comp) const {
  return comp(elem1, elem2);
}

template <typename Key, typename C>
size_t Set<Key, C>::HeightDefine(Node* node) {
  return node ? node->height : 0;
}

template <typename Key, typename C>
Set<Key, C>::Set(const Set& st) {
  std::vector<Key> vec;
  std::stack<Node*> s;
  Node* curr = st.root_;
  while (!s.empty() || curr) {
    while (curr) {
      s.push(curr);
      curr = curr->left;
    }
    Node* popped = s.top();
    s.pop();
    vec.push_back(popped->val);
    curr = popped->right;
  }
  for (size_t i = 0; i < vec.size(); ++i) {
    Insert(vec[i]);
  }
}

template <typename Key, typename C>
Set<Key, C>& Set<Key, C>::operator=(const Set& st) {
  if (root_ == st.root_) {
    return *this;
  }
  Clear();
  std::vector<Key> vec;
  std::stack<Node*> s;
  Node* curr = st.root_;
  while (!s.empty() || curr) {
    while (curr) {
      s.push(curr);
      curr = curr->left;
    }
    Node* popped = s.top();
    s.pop();
    vec.push_back(popped->val);
    curr = popped->right;
  }
  for (size_t i = 0; i < vec.size(); ++i) {
    Insert(vec[i]);
  }
  return *this;
}

template <typename Key, typename C>
int Set<Key, C>::BalanceFactor(Node* node) {
  return static_cast<int>((node->right ? node->right->height : 0) -
                          (node->left ? node->left->height : 0));
}

template <typename Key, typename C>
void Set<Key, C>::HeightFix(Node* node) {
  size_t height_left = (node->left ? node->left->height : 0);
  size_t height_right = (node->right ? node->right->height : 0);
  node->height = (height_left > height_right ? height_left : height_right) + 1;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::Balance(Node* node) {
  HeightFix(node);
  if (BalanceFactor(node) == 2) {
    if (BalanceFactor(node->right) < 0) {
      node->right = RightRotation(node->right);
    }
    return LeftRotation(node);
  }
  if (BalanceFactor(node) == -2) {
    if (BalanceFactor(node->left) > 0) {
      node->left = LeftRotation(node->left);
    }
    return RightRotation(node);
  }
  return node;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::LeftRotation(Node* node) {
  Node* temp = node->right;
  node->right = temp->left;
  temp->left = node;
  HeightFix(node);
  HeightFix(temp);
  return temp;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::RightRotation(Node* node) {
  Node* temp = node->left;
  node->left = temp->right;
  temp->right = node;
  HeightFix(node);
  HeightFix(temp);
  return temp;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::InsertRecursive(Node* root,
                                                         const Key& val) {
  if (root == nullptr) {
    increase_ = true;
    return new Node(val);
  }
  if (Compare(val, root->val)) {
    root->left = InsertRecursive(root->left, val);
  } else if (root->val == val) {
    increase_ = false;
    return Balance(root);
  } else {
    root->right = InsertRecursive(root->right, val);
  }
  return Balance(root);
}

template <typename Key, typename C>
void Set<Key, C>::Insert(const Key& elem) {
  root_ = InsertRecursive(root_, elem);
  if (increase_) {
    ++size_;
  }
}

template <typename Key, typename C>
Set<Key, C>::~Set<Key, C>() {
  ClearRecursive(root_);
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
  size_ = 0;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::FindMin(Node* root) {
  return root->left ? FindMin(root->left) : root;
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::RemoveMin(Node* root) {
  if (root->left == nullptr) {
    return root->right;
  }
  root->left = RemoveMin(root->left);
  return Balance(root);
}

template <typename Key, typename C>
typename Set<Key, C>::Node* Set<Key, C>::EraseRecursive(Node* root,
                                                        const Key& elem) {
  if (root == nullptr) {
    decrease_ = false;
    return nullptr;
  }
  if (elem < root->val) {
    root->left = EraseRecursive(root->left, elem);
  } else if (elem > root->val) {
    root->right = EraseRecursive(root->right, elem);
  } else {
    decrease_ = true;
    Node* left = root->left;
    Node* right = root->right;
    delete root;
    if (right == nullptr) {
      return left;
    }
    Node* min_node = FindMin(right);
    min_node->right = RemoveMin(right);
    min_node->left = left;
    return Balance(min_node);
  }
  return Balance(root);
}

template <typename Key, typename C>
void Set<Key, C>::Erase(const Key& elem) {
  root_ = EraseRecursive(root_, elem);
  if (decrease_) {
    --size_;
  }
}

template <typename Key, typename C>
bool Set<Key, C>::FindElem(Node* node, const Key& val) {
  if (node == nullptr) {
    return false;
  }
  if (node->val == val) {
    return true;
  }
  if (node->val > val) {
    bool res1 = FindElem(node->left, val);
    if (res1) {
      return true;
    }
  }
  bool res2 = FindElem(node->right, val);
  return res2;
}

template <typename Key, typename C>
bool Set<Key, C>::IsFind(const Key& val) {
  return FindElem(root_, val);
}

template <typename Key, typename C>
size_t Set<Key, C>::Size() {
  return size_;
}

template <typename Key, typename C>
void Set<Key, C>::InorderRecursive(Node* root) {
  if (root) {
    InorderRecursive(root->left);
    std::cout << root->val << " ";
    InorderRecursive(root->right);
  }
}
template <typename Key, typename C>
void Set<Key, C>::Inorder() {
  InorderRecursive(root_);
}

template <typename Key, typename C>
bool Set<Key, C>::Empty() const {
  return size_ == 0;
}

template <typename Key, typename C>
Set<Key, C>::ConstIterator::ConstIterator(const std::stack<Node*>& s,
                                            bool check)
: st_(s) {
  if (!st_.empty()) {
    if (check) {
      next_ = st_.top();
      st_.pop();
      current_ = next_;
      next_ = next_->right;
    } else {
      current_ = st_.top();
      st_.pop();
      if (!st_.empty()) {
        next_ = st_.top();
        st_.pop();
        intermediate_ = next_;
        next_ = next_->left;
      }
    }
  }
}

template <typename Key, typename C>
const Key& Set<Key, C>::ConstIterator::operator*() {
  return current_->val;
}

template <typename Key, typename C>
typename Set<Key, C>::ConstIterator& Set<Key, C>::ConstIterator::
operator++() {
  while (next_) {
    st_.push(next_);
    next_ = next_->left;
  }
  if (!st_.empty()) {
    next_ = st_.top();
    st_.pop();
    current_ = next_;
    next_ = next_->right;
  } else {
    current_ = next_ = nullptr;
  }
  return *this;
}

template <typename Key, typename C>
typename Set<Key, C>::ConstIterator& Set<Key, C>::ConstIterator::
operator--() {
  if (intermediate_ && intermediate_->right == nullptr) {
    current_ = intermediate_;
    intermediate_ = nullptr;
    return *this;
  }
  while (next_) {
    st_.push(next_);
    next_ = next_->right;
  }
  if (!st_.empty()) {
    next_ = st_.top();
    st_.pop();
    current_ = next_;
    next_ = next_->left;
  } else {
    current_ = next_ = nullptr;
  }
  return *this;
}

template <typename Key, typename C>
typename Set<Key, C>::ConstIterator& Set<Key, C>::ConstIterator::operator--(
                                                                              int) {
  auto copy = *this;
  --(*this);
  return copy;
}

template <typename Key, typename C>
typename Set<Key, C>::ConstIterator& Set<Key, C>::ConstIterator::operator++(
                                                                              int) {
  auto copy = *this;
  ++(*this);
  return copy;
}

// NOLINT
template <typename Key, typename C>
typename Set<Key, C>::iterator Set<Key, C>::begin() {
  Node* curr = root_;
  std::stack<Node*> s;
  while (curr) {
    s.push(curr);
    curr = curr->left;
  }
  return iterator(s, true);
}

// NOLINT
template <typename Key, typename C>
typename Set<Key, C>::const_iterator Set<Key, C>::cbegin() {
  Node* curr = root_;
  std::stack<Node*> s;
  while (curr) {
    s.push(curr);
    curr = curr->left;
  }
  return const_iterator(s, true);
}

// NOLINT
template <typename Key, typename C>
typename Set<Key, C>::iterator Set<Key, C>::end() {
  Node* curr = root_;
  std::stack<Node*> s;
  while (curr) {
    s.push(curr);
    curr = curr->right;
  }
  s.push(curr);
  return iterator(s, false);
}

// NOLINT
template <typename Key, typename C>
typename Set<Key, C>::const_iterator Set<Key, C>::cend() {
  Node* curr = root_;
  std::stack<Node*> s;
  while (curr) {
    s.push(curr);
    curr = curr->right;
  }
  s.push(curr);
  return const_iterator(s, false);
}

template <typename Key, typename C>
bool Set<Key, C>::const_iterator::operator==(const const_iterator& iter_) const {
  return this->current_ == iter_.current_;
}

template <typename Key, typename C>
bool Set<Key, C>::const_iterator::operator!=(const const_iterator& iter_) const {
  return !(*this == iter_);
}

template <typename Key, typename C>
typename Set<Key, C>::reverse_iterator Set<Key, C>::rbegin() { // NOLINT
  return reverse_iterator(end());
}

template <typename Key, typename C>
typename Set<Key, C>::reverse_iterator Set<Key, C>::rend() { // NOLINT
  return reverse_iterator(begin());
}

template <typename Key, typename C>
typename Set<Key, C>::const_reverse_iterator Set<Key, C>::crbegin() { // NOLINT
  return const_reverse_iterator(cend());
}

template <typename Key, typename C>
typename Set<Key, C>::const_reverse_iterator Set<Key, C>::crend() { // NOLINT
  return const_reverse_iterator(cbegin());
}

template <typename Key, typename C>
bool Set<Key, C>::operator<(Set<Key, C>& st) {
  auto iter1 = begin();
  auto iter2 = st.begin();
  while (iter1 != end() && iter2 != st.end()) {
    if (*iter1 < *iter2) {
      return *iter1 < *iter2;
    }
    ++iter1;
    ++iter2;
  }
  return size_ < st.size_;
}
