#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>

namespace fefu
{
    // Статусы для Node
    enum NodeStatus {
        EMPTY,
        PLACED,
        REMOVED,
        END,
    };
    // Структура, хранит в себе указатель на значение и статус
    template <typename T>
    struct Node {
        T* PtrToValue;
        NodeStatus Status;
        Node() {
            PtrToValue = nullptr;
            Status = EMPTY;
        }
    };

template<typename T>
class allocator {
public:
    // using
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<const T>::type;
    using value_type = T;

    // методы
    allocator() noexcept = default;

    allocator(const allocator&) noexcept = default;

    template <class U>
    allocator(const allocator<U>&) noexcept {};

    ~allocator() = default;

    pointer allocate(size_type n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type) noexcept {
        ::operator delete(p);
    }
}; /// TESTED

template<typename ValueType>
class hash_map_iterator {
public:
    // using
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType&;
    using pointer = ValueType*;

    // friend
    template<typename>
    friend class hash_map_const_iterator;

    // методы
    hash_map_iterator() noexcept = default;

    hash_map_iterator(Node<value_type> &n) noexcept {
        node = &n;
    }

    hash_map_iterator(const hash_map_iterator& other) noexcept {
        node = other.node;
    }

    reference operator*() const {
        if (node == nullptr || node->PtrToValue == nullptr) {
            throw std::out_of_range("node || node->PtrToValue == nullptr");
        }
        else {
            return *(node->PtrToValue);
        }
    }

    pointer operator->() const {
        return node->PtrToValue;
    }

    hash_map_iterator& operator++() {
        ++node;
        while (node == nullptr || (node->Status != PLACED && node->Status != END)) {
            node++;
        }
        return *this;
    }

    hash_map_iterator operator++(int) {
        hash_map_iterator new_this = *this;
        operator++();
        return new_this;
    }

    friend bool operator==(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return (l.node == r.node);
    }

    friend bool operator!=(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // поля
    Node<value_type> *node;
}; /// TESTED

template<typename ValueType>
class hash_map_const_iterator {
// Shouldn't give non const references on value
public:
    // using
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType&;
    using pointer = const ValueType*;

    // методы
    hash_map_const_iterator() noexcept = default;

    hash_map_const_iterator(const Node<value_type> &n) noexcept {
        node = &n;
    }

    hash_map_const_iterator(const hash_map_const_iterator &other) noexcept {
        node = other.node;
    }

    hash_map_const_iterator(const hash_map_iterator<ValueType> &other) noexcept {
        node = other.node;
    }

    reference operator*() const {
        if (node == nullptr || node->PtrToValue == nullptr || node->Status != PLACED) {
            throw std::out_of_range("node || node->PtrToValue == nullptr");
        }
        else {
            return *(node->PtrToValue);
        }
    }

    pointer operator->() const {
        return node->PtrToValue;
    }

    hash_map_const_iterator& operator++() {
        ++node;
        while (node == nullptr || (node->Status != PLACED && node->Status != END)) {
            node++;
        }
        return *this;
    }

    hash_map_const_iterator operator++(int) {
        hash_map_iterator new_this = *this;
        operator++();
        return new_this;
    }

    friend bool operator==(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return (l.node == r.node);
    }

    friend bool operator!=(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // поля
    const Node<value_type> *node;
}; /// TESTED

template<typename K, typename T,
	   typename Hash = std::hash<K>,
	   typename Pred = std::equal_to<K>,
	   typename Alloc = allocator<std::pair<const K, T>>>
class hash_map
{
public:
    // using
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

    // методы
    hash_map() noexcept {
        _Deallocated = true;
        _Size = 0;
        reserve(1);
        _Capacity = 0;
        _LoadFactor = 0.5;
        _Data = nullptr;
    } /// TESTED

    explicit hash_map(size_type n) {
        _Deallocated = true;
        _Size = 0;
        _Capacity = n;
        _LoadFactor = 0.5;
        _Data = nullptr;
        if (n > 0) {
            reserve(n);
        }
    } /// TESTED

    ~hash_map() {
        clear();
    } /// TESTED

    template<typename InputIterator>
    hash_map(InputIterator first, InputIterator last, size_type n = 0) {
        _Deallocated = true;
        _Size = 0;
        _Capacity = n;
        _LoadFactor = 0.5;
        _Data = nullptr;
        if (n > 0) {
            reserve(n);
        }
        for (; first != last; ++first) {
            insert(*first);
        }
    } /// TESTED

    hash_map(const hash_map &hm) {
        _Alloc = hm._Alloc;
        _Deallocated = hm._Deallocated;
        _Size = hm._Size;
        _Capacity = hm._Capacity;
        _LoadFactor = hm._LoadFactor;
        _Data = _Alloc.allocate(_Capacity + 1);
        memcpy(_Data, hm._Data, _Capacity * sizeof(value_type));
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = hm._Hash;
        _Pred = hm._Pred;
    } /// TESTED

    hash_map(hash_map &&hm) {
        using std::move;
        _Alloc = move(hm._Alloc);
        _Deallocated = move(hm._Deallocated);
        _Size = move(hm._Size);
        _Capacity = move(hm._Capacity);
        _LoadFactor = move(hm._LoadFactor);
        _Data = move(hm._Data);
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = move(hm._Hash);
        _Pred = move(hm._Pred);
        hm._Deallocated = true;
        hm.clear();
    } /// TESTED

    explicit hash_map(const allocator_type& a) {
        _Deallocated = true;
        _Size = 0;
        _Capacity = 0;
        _LoadFactor = 0.5;
        _Data = nullptr;
        _Alloc = a;
    } /// TESTED

    hash_map(const hash_map& hm, const allocator_type& a) {
        _Alloc = a;
        _Deallocated = hm._Deallocated;
        _Size = hm._Size;
        _Capacity = hm._Capacity;
        _LoadFactor = hm._LoadFactor;
        _Data = _Alloc.allocate(_Capacity + 1);
        memcpy(_Data, hm._Data, _Capacity * sizeof(value_type));
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = hm._Hash;
        _Pred = hm._Pred;
    } /// TESTED

    hash_map(hash_map &&hm, const allocator_type& a) {
        _Alloc = a;
        _Deallocated = hm._Deallocated;
        _Size = hm._Size;
        _Capacity = hm._Capacity;
        _LoadFactor = hm._LoadFactor;
        _Data = hm._Data;
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = hm._Hash;
        _Pred = hm._Pred;
        hm._Deallocated = true;
        hm.clear();
    } /// TESTED

    hash_map(std::initializer_list<value_type> l, size_type n = 0) {
        _Deallocated = true;
        _Size = 0;
        _Capacity = n;
        _LoadFactor = 0.5;
        _Data = nullptr;
        if (n > 0) {
            reserve(n);
        }
        for (auto el : l) {
            insert(el);
        }
    } /// TESTED

    hash_map& operator=(const hash_map &hm) {
        _Alloc = hm._Alloc;
        _Deallocated = hm._Deallocated;
        _Size = hm._Size;
        _Capacity = hm._Capacity;
        _LoadFactor = hm._LoadFactor;
        _Data = _Alloc.allocate(_Capacity + 1);
        memcpy(_Data, hm._Data, _Capacity * sizeof(value_type));
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = hm._Hash;
        _Pred = hm._Pred;
        return *this;
    } /// TESTED

    hash_map& operator=(hash_map &&hm) {
        using std::move;
        _Alloc = move(hm._Alloc);
        _Deallocated = move(hm._Deallocated);
        _Size = move(hm._Size);
        _Capacity = move(hm._Capacity);
        _LoadFactor = move(hm._LoadFactor);
        _Data = move(hm._Data);
        _Ptr = std::vector<Node<value_type>>(_Ptr);
        _Hash = move(hm._Hash);
        _Pred = move(hm._Pred);
        hm._Deallocated = true;
        hm.clear();
        return *this;
    } /// TESTED

    hash_map& operator=(std::initializer_list<value_type> l) {
        for (auto el : l) {
            insert(el);
        }
        return *this;
    } /// TESTED

    allocator_type get_allocator() const noexcept {
        return _Alloc;
    } /// TESTED

    bool empty() const noexcept {
        return (_Size == 0);
    } /// TESTED

    size_type size() const noexcept {
        return _Size;
    } /// TESTED

    size_type max_size() const noexcept {
        return _Capacity;
    } /// TESTED

    iterator begin() noexcept {
        if (_Capacity == 0) {
            throw std::out_of_range("container_size is 0");
        }
        else {
            int i = -1;
            while (_Ptr[++i].Status != PLACED);
            return iterator(_Ptr[i]);
        }
    } /// TESTED

    const_iterator begin() const noexcept {
        if (_Capacity == 0) {
            throw std::out_of_range("container_size is 0");
        }
        else {
            int i = -1;
            while (_Ptr[++i].Status != PLACED);
            return const_iterator(_Ptr[i]);
        }
    } /// TESTED

    const_iterator cbegin() const noexcept {
        return begin();
    } /// TESTED

    iterator end() noexcept {
        if (_Capacity == 0) {
            throw std::out_of_range("container_size is 0");
        }
        else {
            return iterator(_Ptr[_Capacity]);
        }
    } /// TESTED

    const_iterator end() const noexcept {
        if (_Capacity == 0) {
            throw std::out_of_range("container_size is 0");
        }
        else {
            return const_iterator(_Ptr[_Capacity]);
        }
    } /// TESTED

    const_iterator cend() const noexcept {
        return end();
    } /// TESTED

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

    std::pair<iterator, bool> insert(const value_type& x) {
        if (_Capacity == 0) {
            rehash(ceil(2.0 / _LoadFactor));
        } else
        if ((float)(_Size + 1) / _Capacity > _LoadFactor) {
            rehash(2 * ceil((float)_Size / _LoadFactor));
        }
        int hashed = _Hash(x.first);
        int hash;
        bool unique = true;
        for (int i = 0; i < _Capacity; i++) {
            hash = (abs(hashed) + i) % _Capacity;
            if (_Ptr[hash].Status == EMPTY || _Ptr[hash].Status == REMOVED) {
                new (&_Data[hash]) value_type(x);
                _Ptr[hash].Status = PLACED;
                _Size++;
                break;
            }
            else if (_Ptr[hash].PtrToValue->first == x.first) {
                _Data[hash].second = x.second;
                unique = false;
                break;
            }
        }
        std::pair<iterator, bool> pair(iterator(_Ptr[hash]), unique);
        return pair;
    } /// TESTED

    std::pair<iterator, bool> insert(value_type&& x) {
        if (_Capacity == 0) {
            rehash(ceil(2.0 / _LoadFactor));
        }
        else
            if ((float)(_Size + 1) / _Capacity > _LoadFactor) {
                rehash(2 * ceil((float)_Size / _LoadFactor));
            }
        int hashed = _Hash(x.first);
        int hash;
        bool unique = true;
        for (int i = 0; i < _Capacity; i++) {
            hash = (abs(hashed) + i) % _Capacity;
            if (_Ptr[hash].Status == EMPTY || _Ptr[hash].Status == REMOVED) {
                new (&_Data[hash]) value_type(x);
                _Ptr[hash].Status = PLACED;
                _Size++;
                break;
            }
            else if (_Ptr[hash].PtrToValue->first == x.first) {
                _Data[hash].second = x.second;
                unique = false;
                break;
            }
        }
        std::pair<iterator, bool> pair(iterator(_Ptr[hash]), unique);
        return pair;
    } /// TESTED

    template<typename _InputIterator>
    void insert(_InputIterator first, _InputIterator last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    } /// TESTED

    void insert(std::initializer_list<value_type> l) {
        for (auto el : l) {
            insert(el);
        }
    } /// TESTED


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

    iterator erase(const_iterator position) {
        int place = position.operator->() - _Data;
        iterator it(_Ptr[place]);
        ++it;
        (position.operator->())->~value_type();
        _Ptr[place].Status = REMOVED;
        _Size--;
        return it;
    } /// TESTED

    iterator erase(iterator position) {
        int place = position.operator->() - _Data;
        iterator it(_Ptr[place]);
        ++it;
        (position.operator->())->~value_type();
        _Ptr[place].Status = REMOVED;
        _Size--;
        return it;
    } /// TESTED

    size_type erase(const key_type& x) {
        auto it = find(x);
        if (it == end()) {
            return 0;
        }
        else {
            erase(it);
            return 1;
        }
    } /// TESTED

    iterator erase(const_iterator first, const_iterator last) {
        int b = first.operator->() - _Data;
        int e = last.operator->() - _Data;
        for (int i = b; i < e - 1; ++i) {
            if (_Ptr[i].Status == PLACED) {
                erase(iterator(_Ptr[i]));
            }
        }
        return iterator(_Ptr[e]);
    } /// TESTED

    void clear() noexcept {
        if (!_Deallocated) {
            _Alloc.deallocate(_Data, _Capacity);
            _Ptr.clear();
        }
        _Deallocated = true;
        _Size = 0;
        _Capacity = 0;
        _LoadFactor = 0.5;
    } /// TESTED

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
    void swap(hash_map& x);

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>& source) {
        if (source.size() > 0) {
            for (auto pair : source) {
                insert(pair);
            }
        }
    } /// TESTED

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>&& source) {
        if (source.size() > 0) {
            for (auto pair : source) {
                insert(pair);
            }
        }
    } /// TESTED

    Hash hash_function() const {
        return _Hash;
    } /// TESTED

    Pred key_eq() const {
        return _Pred;
    } /// TESTED

    iterator find(const key_type& x) {
        int hashed = _Hash(x);
        int hash;
        for (int i = 0; i < _Capacity; i++) {
            hash = (abs(hashed) + i) % _Capacity;
            if (_Ptr[hash].Status == EMPTY) {
                return end();
            }
            if (_Ptr[hash].Status == PLACED && _Data[hash].first == x) {
                return iterator(_Ptr[hash]);
            }
        }
        return end();
    } /// TESTED

    const_iterator find(const key_type& x) const {
        int hashed = _Hash(x);
        int hash;
        for (int i = 0; i < _Capacity; i++) {
            hash = (abs(hashed) + i) % _Capacity;
            if (_Ptr[hash].Status == EMPTY) {
                return end();
            }
            if (_Ptr[hash].Status == PLACED && _Data[hash].first == x) {
                return const_iterator(_Ptr[hash]);
            }
        }
        return end();
    } /// TESTED

    size_type count(const key_type& x) const {
        auto it = find(x);
        if (it == end()) {
            return 0;
        }
        else {
            return 1;
        }
    } /// TESTED

    bool contains(const key_type& x) const {
        return count(x);
    } /// TESTED

    mapped_type& operator[](const key_type& k) {
        auto it = find(k);
        if (it == end()) {
            auto elem = insert({ k, mapped_type() });
            return (*(elem.first)).second;
        }
        else {
            return (*it).second;
        }
    } /// TESTED

    mapped_type& operator[](key_type&& k) {
        auto it = find(k);
        if (it == end()) {
            auto elem = insert({ k, mapped_type() });
            return (*(elem.first)).second;
        }
        else {
            return (*it).second;
        }
    } /// TESTED

    mapped_type& at(const key_type& k) {
        auto it = find(k);
        if (it == end()) {
            throw std::out_of_range("no key");
        }
        return (*it).second;
    } /// TESTED

    const mapped_type& at(const key_type& k) const {
        auto it = find(k);
        if (it == end()) {
            throw std::out_of_range("no key");
        }
        return (*it).second;
    } /// TESTED

    size_type bucket_count() const noexcept {
        return _Size;
    } /// TESTED

    size_type bucket(const key_type& _K) const {
        auto it = find(_K);
        if (it == end()) {
            return -1;
        }
        else {
            return it.operator->() - _Data;
        }
    } /// TESTED

    float load_factor() const noexcept {
        if (_Size == 0) {
            return 0;
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
    } /// TESTED

    void rehash(size_type NewCapacity) {
        if ((float)_Size / NewCapacity <= _LoadFactor) {
            value_type* Data = _Data;
            std::vector<Node<value_type>> Ptr(_Ptr);
            _Capacity = NewCapacity;
            _Data = _Alloc.allocate(_Capacity);
            _Ptr = std::vector<Node<value_type>>(_Capacity + 1);
            _Ptr[_Capacity].Status = END;
            _Size = 0;
            for (int i = 0; i < _Capacity + 1; i++) {
                _Ptr[i].PtrToValue = &_Data[i];
            }
            for (int i = 0; i < Ptr.size(); i++) {
                if (Ptr[i].Status == PLACED) {
                    insert({ Ptr[i].PtrToValue->first, Ptr[i].PtrToValue->second });
                }
            }
            _Deallocated = false;
            _Alloc.deallocate(Data, Ptr.size());
            Ptr.clear();
        }
    } /// TESTED

    void reserve(size_type n) {
        rehash(ceil(n / _LoadFactor));
    } /// TESTED

    bool operator==(const hash_map& other) const {
        return true;
    } // fix me

private:
    // поля
    bool _Deallocated; // освобождена память?
    int _Size; // количество элементов
    int _Capacity; // вместительность 
    float _LoadFactor; // коэффициент заполненности
    std::vector<Node<value_type>> _Ptr; // информация об элементах _Data
    value_type *_Data; // указатель на память
    Hash _Hash; // хеш функция
    Pred _Pred; // функция сравнения
    Alloc _Alloc; // аллокатор
};

} // namespace fefu
