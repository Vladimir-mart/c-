
#include <iostream>

template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
  struct Node {
    T value;
    Node* next = nullptr;
    Node* prev = nullptr;

    Node(Node* prev = nullptr, Node* next = nullptr) : next(next), prev(prev) {}
    Node(const T& value, Node* prev = nullptr, Node* next = nullptr)
      : value(value), next(next), prev(prev) {}
  };

  size_t size_ = 0;
  Node* fake_node_;

  using traits = std::allocator_traits<Allocator>;
  using rebind_alloc =
    typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using alloc_traits = typename std::allocator_traits<rebind_alloc>;
  rebind_alloc alloc_;
  Allocator alloc_t_;

  void generate_list(size_t size) {
    this->size_ = size;
    fake_node_ = alloc_traits::allocate(alloc_, 1);
    fake_node_->prev = fake_node_->next = fake_node_;

    if (size == 0) {
      return;
    }

    Node* prev = fake_node_;
    for (size_t i = 0; i < size; ++i) {
      Node* tmp = alloc_traits::allocate(alloc_, 1);
      prev->next = tmp;
      tmp->prev = prev;
      fake_node_->prev = tmp;
      tmp->next = fake_node_;
      prev = tmp;
    }
  }

  Node* generate_new_fake_node(size_t size, rebind_alloc& temp_alloc) {
    Node* new_fake_node = alloc_traits::allocate(temp_alloc, 1);
    new_fake_node->prev = new_fake_node->next = new_fake_node;

    if (size == 0) {
      return new_fake_node;
    }

    Node* prev = new_fake_node;
    for (size_t i = 0; i < size; ++i) {
      Node* tmp = alloc_traits::allocate(temp_alloc, 1);
      prev->next = tmp;
      tmp->prev = prev;
      new_fake_node->prev = tmp;
      tmp->next = new_fake_node;
      prev = tmp;
    }
    return new_fake_node;
  }

  void put_after(Node* pointer, const T& value) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    alloc_traits::construct(alloc_, tmp, value, pointer, pointer->next);
    pointer->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }

  void put_before(Node* pointer, const T& value) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    alloc_traits::construct(alloc_, tmp, value, pointer->prev, pointer);
    pointer->prev = tmp;
    tmp->prev->next = tmp;
    ++size_;
  }

  void put_after(Node* pointer, Node* tmp) {
    tmp->prev = pointer;
    tmp->next = pointer->next;
    pointer->next = tmp;
    tmp->next->prev = tmp;
    ++size_;
  }

  void put_before(Node* pointer, Node* tmp) {
    tmp->next = pointer;
    tmp->prev = pointer->prev;
    pointer->prev = tmp;
    tmp->prev->next = tmp;
    ++size_;
  }

  void delete_node(Node* pointer) {
    assert(pointer != fake_node_);

    pointer->prev->next = pointer->next;
    pointer->next->prev = pointer->prev;
    alloc_traits::destroy(alloc_, pointer);
    alloc_traits::deallocate(alloc_, pointer, 1);
    --size_;
  }

  void destroy() {
    if (empty()) {
      alloc_traits::deallocate(alloc_, fake_node_, 1);
      return;
    }
    Node* iter = fake_node_->next;
    alloc_traits::deallocate(alloc_, fake_node_, 1);
    fake_node_ = iter;
    for (size_t i = 0; i < size_; ++i) {
      Node* tmp = fake_node_;
      fake_node_ = fake_node_->next;
      alloc_traits::destroy(alloc_, tmp);
      alloc_traits::deallocate(alloc_, tmp, 1);
    }
    fake_node_ = nullptr;
    size_ = 0;
  }

public:
  using value_type = T;
  using allocator_type = Allocator;

  explicit List(const Allocator& alloc = Allocator()) : alloc_(alloc) {
    generate_list(0);
  }

  explicit List(size_t count, const T& value,
    const Allocator& alloc = Allocator())
    : alloc_(alloc) {
    generate_list(count);
    Node* iter = fake_node_->next;
    try {
      for (; iter != fake_node_; iter = iter->next) {
        alloc_traits::construct(alloc_, iter, value, iter->prev, iter->next);
      }
    }
    catch (...) {
      Node* iter1 = fake_node_->next;
      Node* titer1 = fake_node_->next;
      int test = 0;
      for (size_t i = 0; i < size_; ++i) {
        titer1 = iter1->next;
        if (iter1 == iter) {
          test = 1;
        }
        if (iter1 != iter && test == 0) {
          alloc_traits::destroy(alloc_, iter1);
        }
        alloc_traits::deallocate(alloc_, iter1, 1);
        iter1 = titer1;
        // alloc_traits::deallocate(alloc_, iter1, 1);
      }
      alloc_traits::deallocate(alloc_, iter1, 1);
      fake_node_ = nullptr;
      size_ = 0;
      throw;
    }
  }

  explicit List(size_t count, const Allocator& alloc = Allocator())
    : alloc_(alloc) {
    generate_list(count);
    Node* iter = fake_node_->next;
    try {
      for (; iter != fake_node_; iter = iter->next) {
        alloc_traits::construct(alloc_, iter, iter->prev, iter->next);
      }
    }
    catch (...) {
      Node* iter1 = fake_node_->next;
      Node* titer1 = fake_node_->next;
      int test = 0;
      for (size_t i = 0; i < size_; ++i) {
        titer1 = iter1->next;
        if (iter1 == iter) {
          test = 1;
        }
        if (iter1 != iter && test == 0) {
          alloc_traits::destroy(alloc_, iter1);
        }
        alloc_traits::deallocate(alloc_, iter1, 1);
        iter1 = titer1;
        // alloc_traits::deallocate(alloc_, iter1, 1);
      }
      alloc_traits::deallocate(alloc_, iter1, 1);
      fake_node_ = nullptr;
      size_ = 0;
      throw;
    }
  }

  List(const List& other)
    : alloc_(
      std::allocator_traits<Allocator>::
      select_on_container_copy_construction(other.get_allocator())) {
    generate_list(other.size_);
    Node* current = fake_node_->next;
    Node* other_current = other.fake_node_->next;
    try {
      for (; current != fake_node_;
        current = current->next, other_current = other_current->next) {
        alloc_traits::construct(alloc_, current, other_current->value,
          current->prev, current->next);
      }
    }
    catch (...) {
      Node* iter1 = fake_node_->next;
      Node* titer1 = fake_node_->next;
      int test = 0;
      for (size_t i = 0; i < size_; ++i) {
        titer1 = iter1->next;
        if (iter1 == current) {
          test = 1;
        }
        if (iter1 != current && test == 0) {
          alloc_traits::destroy(alloc_, iter1);
        }
        alloc_traits::deallocate(alloc_, iter1, 1);
        iter1 = titer1;
        // alloc_traits::deallocate(alloc_, iter1, 1);
      }
      alloc_traits::deallocate(alloc_, iter1, 1);
      fake_node_ = nullptr;
      size_ = 0;
      throw;
    }
  }

  List(std::initializer_list<T> init_list, const Allocator& alloc = Allocator())
    : alloc_(alloc) {
    generate_list(init_list.size());
    Node* current = fake_node_->next;
    try {
      for (const T& node : init_list) {
        if (current == fake_node_) {
          break;
        }
        alloc_traits::construct(alloc_, current, node, current->prev,
          current->next);
        current = current->next;
      }
    }
    catch (...) {
      Node* iter1 = fake_node_->next;
      Node* titer1 = fake_node_->next;
      int test = 0;
      for (size_t i = 0; i < size_; ++i) {
        titer1 = iter1->next;
        if (iter1 == current) {
          test = 1;
        }
        if (iter1 != current && test == 0) {
          alloc_traits::destroy(alloc_, iter1);
        }
        alloc_traits::deallocate(alloc_, iter1, 1);
        iter1 = titer1;
        // alloc_traits::deallocate(alloc_, iter1, 1);
      }
      alloc_traits::deallocate(alloc_, iter1, 1);
      fake_node_ = nullptr;
      size_ = 0;
      throw;
    }
  }

  // void try_make_new(const List& other) {
  //   Node* old_fake = fake_node_;
  //   size_t old_size = size_;
  //   init();
  //   try {
  //     for (auto& i : other) {
  //       Node* new_n = alloc_.allocate(1);
  //       try {
  //         alloc_traits::construct(alloc_, new_n, i);
  //       } catch (...) {
  //         alloc_.deallocate(new_n, 1);
  //         throw;
  //       }
  //       insert_before(fake_node_->prev, new_n);
  //     }
  //   } catch (...) {
  //     destroy();
  //     fake_node_ = old_fake;
  //     size_ = old_size;
  //     throw;
  //   }
  //   Node* new_first = first_;
  //   Node* new_last = last_;
  //   first_ = old_first;
  //   last_ = old_last;
  //   destroy();
  //   first_ = new_first;
  //   last_ = new_last;
  // }

  // List& operator=(const List& other) {
  // if (this == &other) {
  //   return *this;
  // }
  // if (alloc_traits::propagate_on_container_copy_assignment::value &&
  //     alloc_ != other.alloc_) {
  //   alloc_ = other.alloc_;
  // }
  // try_make_new(other);
  // size_ = other.size_;
  // return *this;
  // }

  // List& operator=(const List& other) {
  //   if (this == &other) {
  //     return *this;
  //   }
  //   destroy();
  //   if (std::allocator_traits<
  //           Allocator>::propagate_on_container_copy_assignment::value) {
  //     alloc_ = other.alloc_;
  //   }
  //   generate_list(other.size_);
  //   Node* current = fake_node_->next;
  //   Node* current_other = other.fake_node_->next;
  //   for (; current != fake_node_;
  //        current = current->next, current_other = current_other->next) {
  //     alloc_traits::construct(alloc_, current, current_other->value,
  //                             current->prev, current->next);
  //   }
  //   return *this;
  // }

  List& operator=(const List& other) {
    if (this == &other) {
      return *this;
    }
    // destroy();
    rebind_alloc temp_alloc = alloc_;
    if (std::allocator_traits<
      Allocator>::propagate_on_container_copy_assignment::value) {
      temp_alloc = other.alloc_;
    }
    // generate_list(other.size_);
    Node* new_fake_node = generate_new_fake_node(other.size(), temp_alloc);
    Node* current = new_fake_node->next;
    Node* current_other = other.fake_node_->next;
    try {
      for (; current != new_fake_node;
        current = current->next, current_other = current_other->next) {
        alloc_traits::construct(temp_alloc, current, current_other->value,
          current->prev, current->next);
      }
    }
    catch (...) {
      Node* iter1 = new_fake_node->next;
      Node* titer1 = new_fake_node->next;
      int test = 0;
      for (size_t i = 0; i < other.size(); ++i) {
        titer1 = iter1->next;
        if (iter1 == current) {
          test = 1;
        }
        if (iter1 != current && test == 0) {
          alloc_traits::destroy(temp_alloc, iter1);
        }
        alloc_traits::deallocate(temp_alloc, iter1, 1);
        iter1 = titer1;
        // alloc_traits::deallocate(alloc_, iter1, 1);
      }
      alloc_traits::deallocate(temp_alloc, iter1, 1);
      new_fake_node = nullptr;
      throw;
    }
    destroy();
    fake_node_ = new_fake_node;
    size_ = other.size_;
    new_fake_node = nullptr;
    fake_node_->prev->next = fake_node_;
    fake_node_->next->prev = fake_node_;
    alloc_ = temp_alloc;
    return *this;
  }

  List& operator=(List&& other) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this == &other) {
      return *this;
    }
    destroy();
    size_ = other.size_;
    fake_node_ = other.fake_node_;
    if (alloc_ != other.alloc_ &&
      std::allocator_traits<
      Allocator>::propagate_on_container_copy_assignment::value) {
      alloc_ = std::move(other.alloc_);
    }
    other.size_ = 0;
    other.fake_node_ = alloc_traits::allocate(other.alloc_, 1);
    other.fake_node_->prev = other.fake_node_->next = other.fake_node_;
    return *this;
  }

  Allocator get_allocator() const { return alloc_; }

  bool empty() { return size_ == 0; }

  [[nodiscard]] size_t size() const { return size_; }

  void push_back(const T& value) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, &tmp->value, value);
      put_after(fake_node_->next, tmp);
    }
    catch (...) {
      alloc_traits::deallocate(alloc_, tmp, 1);
      throw;
    }
  }

  // void push_back(T&& value) { //emplace_back(std::move(value)); 
  //   emplace_back(value);
  // }

  void pop_back() { delete_node(fake_node_->prev); }

  void push_front(const T& value) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, &tmp->value, value);
      put_before(fake_node_->next, tmp);
    }
    catch (...) {
      alloc_traits::deallocate(alloc_, tmp, 1);
      throw;
    }
  }

  void push_front(T&& value) { emplace_front(std::move(value)); }

  void pop_front() { delete_node(fake_node_->next); }

  T& front() { return fake_node_->next->value; }

  const T& front() const { return fake_node_->next->value; }

  T& back() { return fake_node_->prev->value; }

  const T& back() const { return fake_node_->prev->value; }

  template <bool IsConst>
  class CommonIterator {
  private:
    using current_type = std::conditional_t<IsConst, const T, T>;
    using current_reference = std::conditional_t<IsConst, const T&, T&>;
    using current_pointer = std::conditional_t<IsConst, const T*, T*>;

    Node* node_;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;
    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using difference_type = std::ptrdiff_t;

    CommonIterator() = default;
    CommonIterator(Node* node = nullptr) : node_(node) {}

    // CommonIterator& operator=(const CommonIterator& other) {
    //   node_ = other.node_;
    //   return *this;
    // }
    CommonIterator(const CommonIterator& other) = default;

    CommonIterator& operator++() {
      node_ = node_->next;
      return *this;
    }

    CommonIterator& operator--() {
      node_ = node_->prev;
      return *this;
    }

    CommonIterator operator++(int) {
      CommonIterator ans = *this;
      ++(*this);
      return ans;
    }

    CommonIterator operator--(int) {
      CommonIterator ans = *this;
      --(*this);
      return ans;
    }

    current_reference operator*() const { return node_->value; }

    current_pointer operator->() const { return &(node_->value); }

    template <bool OtherConst>
    friend bool operator==(const CommonIterator<IsConst>& other1,
      const CommonIterator<OtherConst>& other2) {
      return other2.get_node() == other1.get_node();
    }

    template <bool OtherConst>
    friend bool operator!=(const CommonIterator<IsConst>& other1,
      const CommonIterator<OtherConst>& other2) {
      return !(other2 == other1);
    }

    Node* get_node() const { return node_; }

    CommonIterator base() { return CommonIterator<IsConst>(node_->next); }
  };

  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  reverse_iterator rbegin() const {
    return const_reverse_iterator(fake_node_->prev);
  }
  reverse_iterator rend() const { return const_reverse_iterator(fake_node_); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(fake_node_->prev);
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(fake_node_);
  }

  iterator begin() const { return fake_node_->next; }

  iterator end() { return fake_node_; }

  const_iterator cbegin() const { return fake_node_->next; }

  const_iterator cend() const { return fake_node_; }

  const_iterator end() const { return fake_node_; }

  template <bool IsConst>
  void insert(CommonIterator<IsConst> iter, const T& value) {
    put_after(iter.get_node()->prev, value);
  }

  template <bool IsConst>
  void erase(CommonIterator<IsConst> iter) {
    delete_node(iter.get_node());
  }

  template <class... Args>
  void push_back(Args&&... args) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, &tmp->value, std::forward<Args>(args)...);
    }
    catch (...) {
      alloc_traits::deallocate(alloc_, tmp, 1);
      throw;
    }
    put_after(fake_node_->prev, tmp);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    Node* tmp = alloc_traits::allocate(alloc_, 1);
    try {
      alloc_traits::construct(alloc_, &tmp->value, std::forward<Args>(args)...);
      put_before(fake_node_->next, tmp);
    }
    catch (...) {
      alloc_traits::deallocate(alloc_, tmp, 1);
      throw;
    }
  }

  ~List() { destroy(); }
};

static int d_calls = 0;
static int c_calls = 0;
struct Account {
  int b = 0;
  Account() = default;
  Account(const Account& acc) : b(acc.b) {
    ++c_calls;
    if (c_calls == 5) {
      try {
        throw std::runtime_error("");
      }
      catch (...) {
        throw;
      }
    }
  }
  Account(int a) : b(a) {}
  ~Account() { ++d_calls; }
};
