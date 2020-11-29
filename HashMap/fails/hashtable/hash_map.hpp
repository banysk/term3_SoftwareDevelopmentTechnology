#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <vector>
#include <new>

namespace fefu
{
    enum NodeStatus {
        EMPTY,
        PLACED,
        DELETED
    };

    template<typename T>
    struct Node {
        T* ptr;
        NodeStatus status;
        Node() {
            this->ptr = nullptr;
            this->status = EMPTY;
        }
    };

template<typename T>
class allocator {
public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<const T>::type;
    using value_type = T;
    // methods
    allocator() noexcept = default;
    allocator(const allocator&) noexcept = default;
    template <class U>
    allocator(const allocator<U> &alloc) noexcept;
    ~allocator() = default;
    pointer allocate(size_type n) {
        pointer ptr = static_cast<pointer>(::operator new(n * sizeof(value_type)));
        return ptr;
    }
    void deallocate(pointer p, size_type n) noexcept {
        ::operator delete(static_cast<void*>(p), n * sizeof(value_type));
    }
}; /// TESTED

template<typename ValueType>
class hash_map_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType&;
    using pointer = ValueType*;
    // methods
    hash_map_iterator() noexcept {
        node = nullptr;
    }
    hash_map_iterator(const hash_map_iterator &other) noexcept {
        node = other.node;
    }
    hash_map_iterator(Node<value_type> &n) {
        node = &n;
    }
    reference operator*() const {
        if (node == nullptr || node->ptr == nullptr) {
            std::out_of_range("nullptr");
        }
        else {
            return *(node->ptr);
        }
    }
    pointer operator->() const {
        if (node == nullptr) {
            throw std::out_of_range("nullptr");
        }
        else {
            return node->ptr;
        }
    }
    hash_map_iterator& operator++() {
        node++;
        return *this;
    };
    hash_map_iterator operator++(int) {
        hash_map_iterator _this = *this;
        node++;
        return _this;
    }
    friend bool operator==(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return (l.node == r.node);
    }
    friend bool operator!=(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // fields
    Node<value_type> *node;
}; /// TESTED

template<typename ValueType>
class hash_map_const_iterator {
// Shouldn't give non const references on value
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType&;
    using pointer = const ValueType*;
    // methods
    hash_map_const_iterator() noexcept {
        node = nullptr;
    }
    hash_map_const_iterator(const hash_map_const_iterator& other) noexcept {
        node = other.node;
    }
    hash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept {
        node = other.node;
    }
    hash_map_const_iterator(Node<value_type> &n) {
        node = &n;
    }
    reference operator*() const {
        if (node == nullptr || node->ptr == nullptr) {
            std::out_of_range("nullptr");
        }
        else {
            return *(node->ptr);
        }
    }
    pointer operator->() const {
        if (node == nullptr) {
            throw std::out_of_range("nullptr");
        }
        else {
            return node->ptr;
        }
    }
    hash_map_const_iterator& operator++() {
        node++;
        return *this;
    }
    hash_map_const_iterator operator++(int) {
        hash_map_const_iterator _this = *this;
        node++;
        return _this;
    }
    friend bool operator==(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return (l.node == r.node);
    }
    friend bool operator!=(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // fields
    Node<value_type> *node;
}; /// TESTED

template<typename K, typename T,
	   typename Hash = std::hash<K>,
	   typename Pred = std::equal_to<K>,
	   typename Alloc = allocator<std::pair<const K, T>>>
class hash_map
{
public:
    using key_type = K;
    using mapped_type = T;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Alloc;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = hash_map_iterator<value_type>;
    using const_iterator = hash_map_const_iterator<value_type>;
    using size_type = std::size_t;
    // methods
    hash_map() {
        _LoadFactor = 0.9;
        _Size = 0;
        _Capacity = 0;
    } /// TESTED
    explicit hash_map(size_type n) {
        _LoadFactor = 0.9;
        _Size = 0;
        _Capacity = n;
        _Ptr.resize(n);
        _Data = _Alloc.allocate(n);
        for (int i = 0; i < _Capacity; i++) {
            _Ptr[i].ptr = (_Data + i);
        }
    } /// TESTED

    /**
     *  @brief  Builds an %hash_map from a range.
     *  @param  first  An input iterator.
     *  @param  last  An input iterator.
     *  @param  n  Minimal initial number of buckets.
     *
     *  Create an %hash_map consisting of copies of the elements from
     *  [first,last).  This is linear in N (where N is
     *  distance(first,last)).
     */
    template<typename InputIterator>
    hash_map(InputIterator first, InputIterator last, size_type n = 0);

    hash_map(const hash_map &hm) {
        _LoadFactor = hm._LoadFactor;
        _Size = hm._Size;
        _Capacity = hm._Capacity;
        _Hash = hm._Hash;
        _Pred = hm._Pred;
        _Alloc = hm._Alloc;
        _Ptr = hm._Ptr;
        _Data = hm._Data;
        for (int i = 0; i < _Capacity; i++) {
            _Ptr[i].ptr = (_Data + i);
        }
    } /// TESTED
    hash_map(hash_map &&hm) {
        _LoadFactor = std::move(hm._LoadFactor);
        _Size = std::move(hm._Size);
        _Capacity = std::move(hm._Capacity);
        _Hash = std::move(hm._Hash);
        _Pred = std::move(hm._Pred);
        _Alloc = std::move(hm._Alloc);
        _Ptr = std::move(hm._Ptr);
        _Data = std::move(hm._Data);
    } /// TESTED
    explicit hash_map(const allocator_type& a) {
        _LoadFactor = 0.9;
        _Size = 0;
        _Capacity = 0;
        _Alloc = a;
    } /// TESTED

    /*
    *  @brief Copy constructor with allocator argument.
    * @param  uset  Input %hash_map to copy.
    * @param  a  An allocator object.
    */
    hash_map(const hash_map& umap, const allocator_type& a);

    /*
    *  @brief  Move constructor with allocator argument.
    *  @param  uset Input %hash_map to move.
    *  @param  a    An allocator object.
    */
    hash_map(hash_map&& umap, const allocator_type& a);

    /**
     *  @brief  Builds an %hash_map from an initializer_list.
     *  @param  l  An initializer_list.
     *  @param n  Minimal initial number of buckets.
     *
     *  Create an %hash_map consisting of copies of the elements in the
     *  list. This is linear in N (where N is @a l.size()).
     */
    hash_map(std::initializer_list<value_type> l, size_type n = 0);

    /// Copy assignment operator.
    hash_map& operator=(const hash_map&);

    /// Move assignment operator.
    hash_map& operator=(hash_map&&);

    /**
     *  @brief  %hash_map list assignment operator.
     *  @param  l  An initializer_list.
     *
     *  This function fills an %hash_map with copies of the elements in
     *  the initializer list @a l.
     *
     *  Note that the assignment completely changes the %hash_map and
     *  that the resulting %hash_map's size is the same as the number
     *  of elements assigned.
     */
    hash_map& operator=(std::initializer_list<value_type> l);

    allocator_type get_allocator() const noexcept {
        return _Alloc;
    } /// TESTED

    // size and capacity:
    bool empty() const noexcept {
        return _Size == 0;
    } /// TESTED
    size_type size() const noexcept {
        return _Size;
    } /// TESTED
    size_type max_size() const noexcept {
        return _Capacity;
    } /// TESTED

    // iterators.
    iterator begin() noexcept {
        return iterator(_Ptr[0]);
    } /// TESTED

    //@{
    /**
     *  Returns a read-only (constant) iterator that points to the first
     *  element in the %hash_map.
     */
    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;
    iterator end() noexcept {
        return iterator(_Ptr[_Capacity - 1]);
    } /// TESTED

    //@{
    /**
     *  Returns a read-only (constant) iterator that points one past the last
     *  element in the %hash_map.
     */
    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;
    //@}

    // modifiers.

    /**
     *  @brief Attempts to build and insert a std::pair into the
     *  %hash_map.
     *
     *  @param args  Arguments used to generate a new pair instance (see
     *	        std::piecewise_contruct for passing arguments to each
    *	        part of the pair constructor).
    *
    *  @return  A pair, of which the first element is an iterator that points
    *           to the possibly inserted pair, and the second is a bool that
    *           is true if the pair was actually inserted.
    *
    *  This function attempts to build and insert a (key, value) %pair into
    *  the %hash_map.
    *  An %hash_map relies on unique keys and thus a %pair is only
    *  inserted if its first element (the key) is not already present in the
    *  %hash_map.
    *
    *  Insertion requires amortized constant time.
    */
    template<typename... _Args>
    std::pair<iterator, bool> emplace(_Args&&... args);

    /**
     *  @brief Attempts to build and insert a std::pair into the
     *  %hash_map.
     *
     *  @param k    Key to use for finding a possibly existing pair in
     *                the hash_map.
     *  @param args  Arguments used to generate the .second for a
     *                new pair instance.
     *
     *  @return  A pair, of which the first element is an iterator that points
     *           to the possibly inserted pair, and the second is a bool that
     *           is true if the pair was actually inserted.
     *
     *  This function attempts to build and insert a (key, value) %pair into
     *  the %hash_map.
     *  An %hash_map relies on unique keys and thus a %pair is only
     *  inserted if its first element (the key) is not already present in the
     *  %hash_map.
     *  If a %pair is not inserted, this function has no effect.
     *
     *  Insertion requires amortized constant time.
     */
    template <typename... _Args>
    std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args);

    // move-capable overload
    template <typename... _Args>
    std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args);

    //@{
    /**
     *  @brief Attempts to insert a std::pair into the %hash_map.
     *  @param x Pair to be inserted (see std::make_pair for easy
     *	     creation of pairs).
    *
    *  @return  A pair, of which the first element is an iterator that
    *           points to the possibly inserted pair, and the second is
    *           a bool that is true if the pair was actually inserted.
    *
    *  This function attempts to insert a (key, value) %pair into the
    *  %hash_map. An %hash_map relies on unique keys and thus a
    *  %pair is only inserted if its first element (the key) is not already
    *  present in the %hash_map.
    *
    *  Insertion requires amortized constant time.
    */
    std::pair<iterator, bool> insert(const value_type& x) {
        if ((float)(_Size + 1) / _Capacity >= _LoadFactor) {
            rehash(ceil(((float)_Size + 1) / _LoadFactor));
        }
        int hash = std::hash<K>()(x.first) % _Capacity;
        int cur_hash = abs(hash);
        bool unique = true;
        int i;
        for (i = 0; i < _Ptr.size(); i++) {
            if (_Ptr[cur_hash].status == EMPTY) {
                _Ptr[cur_hash].status = PLACED;
                _Size++;
                break;
            }
            if (_Ptr[cur_hash].status == PLACED && _Ptr[cur_hash].ptr->first == x.first) {
                unique = false;
                break;
            }
        }
        memcpy(&_Data[cur_hash], &x, sizeof(x));
        return std::make_pair(iterator(_Ptr[cur_hash]), unique);
    }

    std::pair<iterator, bool> insert(value_type&& x) {
        if ((float)(_Size + 1) / _Capacity >= _LoadFactor) {
            rehash(2 * ceil(((float)_Size + 1) / _LoadFactor));
        }
        int hash = std::hash<K>()(x.first) % _Capacity;
        int cur_hash = abs(hash);
        bool unique = true;
        int i;
        for (i = 0; i < _Ptr.size(); i++) {
            if (_Ptr[cur_hash].status == EMPTY) {
                _Ptr[cur_hash].status = PLACED;
                _Size++;
                break;
            }
            if (_Ptr[cur_hash].status == PLACED && _Ptr[cur_hash].ptr->first == x.first) {
                unique = false;
                break;
            }
        }
        memcpy(&_Data[cur_hash], &x, sizeof(x));
        return std::make_pair(iterator(_Ptr[cur_hash]), unique);
    }

    //@}

    /**
     *  @brief A template function that attempts to insert a range of
     *  elements.
     *  @param  first  Iterator pointing to the start of the range to be
     *                   inserted.
     *  @param  last  Iterator pointing to the end of the range.
     *
     *  Complexity similar to that of the range constructor.
     */
    template<typename _InputIterator>
    void insert(_InputIterator first, _InputIterator last);

    /**
     *  @brief Attempts to insert a list of elements into the %hash_map.
     *  @param  l  A std::initializer_list<value_type> of elements
     *               to be inserted.
     *
     *  Complexity similar to that of the range constructor.
     */
    void insert(std::initializer_list<value_type> l);


    /**
     *  @brief Attempts to insert a std::pair into the %hash_map.
     *  @param k    Key to use for finding a possibly existing pair in
     *                the map.
     *  @param obj  Argument used to generate the .second for a pair
     *                instance.
     *
     *  @return  A pair, of which the first element is an iterator that
     *           points to the possibly inserted pair, and the second is
     *           a bool that is true if the pair was actually inserted.
     *
     *  This function attempts to insert a (key, value) %pair into the
     *  %hash_map. An %hash_map relies on unique keys and thus a
     *  %pair is only inserted if its first element (the key) is not already
     *  present in the %hash_map.
     *  If the %pair was already in the %hash_map, the .second of
     *  the %pair is assigned from obj.
     *
     *  Insertion requires amortized constant time.
     */
    template <typename _Obj>
    std::pair<iterator, bool> insert_or_assign(const key_type& k, _Obj&& obj);

    // move-capable overload
    template <typename _Obj>
    std::pair<iterator, bool> insert_or_assign(key_type&& k, _Obj&& obj);

    //@{
    /**
     *  @brief Erases an element from an %hash_map.
     *  @param  position  An iterator pointing to the element to be erased.
     *  @return An iterator pointing to the element immediately following
     *          @a position prior to the element being erased. If no such
     *          element exists, end() is returned.
     *
     *  This function erases an element, pointed to by the given iterator,
     *  from an %hash_map.
     *  Note that this function only erases the element, and that if the
     *  element is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(const_iterator position);

    // LWG 2059.
    iterator erase(iterator position);
    //@}

    /**
     *  @brief Erases elements according to the provided key.
     *  @param  x  Key of element to be erased.
     *  @return  The number of elements erased.
     *
     *  This function erases all the elements located by the given key from
     *  an %hash_map. For an %hash_map the result of this function
     *  can only be 0 (not present) or 1 (present).
     *  Note that this function only erases the element, and that if the
     *  element is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    size_type erase(const key_type& x);

    /**
     *  @brief Erases a [first,last) range of elements from an
     *  %hash_map.
     *  @param  first  Iterator pointing to the start of the range to be
     *                  erased.
     *  @param last  Iterator pointing to the end of the range to
     *                be erased.
     *  @return The iterator @a last.
     *
     *  This function erases a sequence of elements from an %hash_map.
     *  Note that this function only erases the elements, and that if
     *  the element is itself a pointer, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(const_iterator first, const_iterator last);

    /**
     *  Erases all elements in an %hash_map.
     *  Note that this function only erases the elements, and that if the
     *  elements themselves are pointers, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    void clear() noexcept;

    /**
     *  @brief  Swaps data with another %hash_map.
     *  @param  x  An %hash_map of the same element and allocator
     *  types.
     *
     *  This exchanges the elements between two %hash_map in constant
     *  time.
     *  Note that the global std::swap() function is specialized such that
     *  std::swap(m1,m2) will feed to this function.
     */
    void swap(hash_map& x) {
        std::swap(_LoadFactor, x._LoadFactor);
        std::swap(_Size, x._Size);
        std::swap(_Capacity, x._Capacity);
        std::swap(_Hash, x._Hash);
        std::swap(_Pred, x._Pred);
        std::swap(_Alloc, x._Alloc);
        std::swap(_Data, x._Data);
        std::swap(_Ptr, x._Ptr);
    }

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>& source);

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>&& source);

    // observers.
    Hash hash_function() const {
        return _Hash;
    } /// TESTED
    Pred key_eq() const {
        return _Pred;
    } /// TESTED

    // lookup.

    //@{
    /**
     *  @brief Tries to locate an element in an %hash_map.
     *  @param  x  Key to be located.
     *  @return  Iterator pointing to sought-after element, or end() if not
     *           found.
     *
     *  This function takes a key and tries to locate the element with which
     *  the key matches.  If successful the function returns an iterator
     *  pointing to the sought after element.  If unsuccessful it returns the
     *  past-the-end ( @c end() ) iterator.
     */
    iterator find(const key_type& x);

    const_iterator find(const key_type& x) const;
    //@}

    /**
     *  @brief  Finds the number of elements.
     *  @param  x  Key to count.
     *  @return  Number of elements with specified key.
     *
     *  This function only makes sense for %unordered_multimap; for
     *  %hash_map the result will either be 0 (not present) or 1
     *  (present).
     */
    size_type count(const key_type& x) const;

    /**
     *  @brief  Finds whether an element with the given key exists.
     *  @param  x  Key of elements to be located.
     *  @return  True if there is any element with the specified key.
     */
    bool contains(const key_type& x) const;

    //@{
    /**
     *  @brief  Subscript ( @c [] ) access to %hash_map data.
     *  @param  k  The key for which data should be retrieved.
     *  @return  A reference to the data of the (key,data) %pair.
     *
     *  Allows for easy lookup with the subscript ( @c [] )operator.  Returns
     *  data associated with the key specified in subscript.  If the key does
     *  not exist, a pair with that key is created using default values, which
     *  is then returned.
     *
     *  Lookup requires constant time.
     */
    mapped_type& operator[](const key_type& k);

    mapped_type& operator[](key_type&& k);
    //@}

    //@{
    /**
     *  @brief  Access to %hash_map data.
     *  @param  k  The key for which data should be retrieved.
     *  @return  A reference to the data whose key is equal to @a k, if
     *           such a data is present in the %hash_map.
     *  @throw  std::out_of_range  If no such data is present.
     */
    mapped_type& at(const key_type& k);

    const mapped_type& at(const key_type& k) const;

    size_type bucket_count() const noexcept {
        return _Capacity;
    } /// TESTED

    /*
    * @brief  Returns the bucket index of a given element.
    * @param  _K  A key instance.
    * @return  The key bucket index.
    */
    size_type bucket(const key_type& _K) const;

    // hash policy.

    float load_factor() const noexcept {
        if (_Capacity == 0) {
            return 0.0f;
        }
        else {
            return (float)_Size / _Capacity;
        }
    } /// TESTED
    float max_load_factor() const noexcept {
        return _LoadFactor;
    } /// TESTED
    void max_load_factor(float z) {
        float tmp = z / _LoadFactor;
        _LoadFactor = z;
        rehash(ceil(_Capacity / tmp));
    }

    /**
     *  @brief  May rehash the %hash_map.
     *  @param  n The new number of buckets.
     *
     *  Rehash will occur only if the new number of buckets respect the
     *  %hash_map maximum load factor.
     */
    void rehash(size_type n) {
        if ((float)_Size / n < _LoadFactor) {
            hash_map hm(n);
            for (int i = 0; i < _Ptr.size(); i++) {
                if (_Ptr[i].status == PLACED) {
                    hm.insert(*_Ptr[i].ptr);
                }
            }
            swap(hm);
        }
    }
    void reserve(size_type n) {
        if (n > 0) {
            rehash(ceil((float)n / _LoadFactor));
        }
    }

    bool operator==(const hash_map& other) const;

    // fields
private:
    float _LoadFactor;
    int _Size;
    int _Capacity;
    Hash _Hash;
    Pred _Pred;
    Alloc _Alloc;
    value_type *_Data;
    std::vector<Node<value_type>> _Ptr;
};

} // namespace fefu
