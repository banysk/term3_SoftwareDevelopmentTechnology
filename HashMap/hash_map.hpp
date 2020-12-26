#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace fefu
{

enum Status {
    EMPTY,
    PLACED,
    REMOVED,
    END
};

template<typename T>
struct Node {
    T* ceil;
    Status status;
    Node() : ceil(nullptr), status(EMPTY) {}
    Node(T* ceil_, Status status_) : ceil(ceil_), status(status_) {}
    T* operator*() { // T*
        return ceil;
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

    // METHODS
    allocator() noexcept = default;

    allocator(const allocator&) noexcept = default;

    template <class U>
    allocator(const allocator<U>&) noexcept {};
	
    ~allocator() = default;

    pointer allocate(size_type size_) {
        return static_cast<pointer>(::operator new(size_ * sizeof(T)));
    }
	
    void deallocate(pointer ptr_, size_type size_) noexcept {
        ::operator delete(ptr_);
    }
};

template<typename ValueType>
class hash_map_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType&;
    using pointer = ValueType*;

    template<typename>
    friend class hash_map_const_iterator;
    template<typename, typename, typename, typename, typename>
    friend class hash_map;

    // METHODS
    hash_map_iterator() noexcept = default;

    hash_map_iterator(const hash_map_iterator& other) noexcept {
        info = other.info;
    }

    reference operator*() const { // это может вызывать ошибку
        return *(info->ceil);
    }

    pointer operator->() const { // это может вызвать ошибку
        return info->ceil;
    }

    hash_map_iterator& operator++() { // это может вызвать ошибку
        if (info->status == END)
            return *this;
        ++info;
        while (info->status != PLACED && info->status != END) {
            ++info;
        }
        return *this;
    }

    hash_map_iterator operator++(int) { // это может вызвать ошибку
        hash_map_iterator it = *this;
        operator++();
        return it;
    }

    friend bool operator==(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return (l.info == r.info);
    }

    friend bool operator!=(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return (l.info != r.info);
    }

private:
    Node<ValueType> *info;

    hash_map_iterator(Node<ValueType> &info_) noexcept { // to private
        info = &info_;
    }

    hash_map_iterator(Node<ValueType> *info_) noexcept { // to private
        info = info_;
    }
};

template<typename ValueType>
class hash_map_const_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType&;
    using pointer = const ValueType*;

    template<typename, typename, typename, typename, typename>
    friend class hash_map;

    // METHODS
    hash_map_const_iterator() noexcept = default;

    hash_map_const_iterator(const hash_map_const_iterator& other) noexcept {
        info = other.info;
    }

    hash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept {
        info = other.info;
    }

    reference operator*() const { // это может вызвать ошибку
        return *(info->ceil);
    }

    pointer operator->() const { // это может вызвать ошибку
        return info->ceil;
    }

    hash_map_const_iterator& operator++() {
        if (info->status == END)
            return *this;
        ++info;
        while (info->status != PLACED && info->status != END) {
            ++info;
        }
        return *this;
    }

    hash_map_const_iterator operator++(int) {
        hash_map_const_iterator it = *this;
        ::operator++();
        return it;
    }

    friend bool operator==(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return (l.info == r.info);
    }

    friend bool operator!=(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return (l.info != r.info);
    }

private:
    const Node<ValueType> *info;

    hash_map_const_iterator(const Node<ValueType> &info_) noexcept { // to private
        info = &info_;
    }

    hash_map_const_iterator(const Node<ValueType> *info_) noexcept { // to private
        info = info_;
    }
};
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

    // METHODS
    hash_map() : LoadFactor(0.5f), Size(0), Data(nullptr) {
        reserve(1);
    } /// OK

    explicit hash_map(size_type n) : LoadFactor(0.5f), Size(0), Data(nullptr) {
        if (n > 1) {
            reserve(n);
        }
        else {
            reserve(1);
        }
    } /// OK

    template<typename InputIterator>
    hash_map(InputIterator first, InputIterator last, size_type n = 0) : LoadFactor(0.5f), Size(0), Data(nullptr) {
        if (n > 1) {
            reserve(n);
        }
        else {
            reserve(1);
        }
        insert(first, last);
    } /// OK

    void copy_constructor_wrapper(const hash_map &hm) {
        LoadFactor = hm.LoadFactor;
        Capacity = hm.Capacity;
        Size = hm.Size;
        // =====
        Hash_ = hm.Hash_;
        Pred_ = hm.Pred_;
        Allocator = hm.Allocator;
        // =====
        Info.resize(Capacity + 1);
        Data = Allocator.allocate(Capacity + 1);
        for (int i = 0; i < Capacity + 1; i++) {
            Info[i] = Node<value_type>(&Data[i], hm.Info[i].status);
            if (hm.Info[i].status == PLACED) {
                new (&Data[i]) value_type(hm.Data[i]);
            }
        }
    } /// OK

    hash_map(const hash_map &hm) {
        copy_constructor_wrapper(hm);
    } /// OK

    hash_map(hash_map &&hm) {
        move(hm);
    } /// OK

    explicit hash_map(const allocator_type& a) {
        LoadFactor = 0.5f;
        Size = 0;
        Data = nullptr;
        Allocator = a;
        reserve(1);
    } /// OK

    hash_map(const hash_map& hm, const allocator_type& a) {
        copy_constructor_wrapper(hm);
        Allocator = a;
    } /// OK

    hash_map(hash_map&& hm, const allocator_type& a) {
        swap(hm);
        hm.clear();
        Allocator = a;
    } /// OK


    hash_map(std::initializer_list<value_type> l, size_type n = 0) : LoadFactor(0.5f), Size(0), Data(nullptr) {
        if (n > 1) {
            reserve(n);
        }
        else {
            reserve(1);
        }
        for (auto el : l) {
            insert(el);
        }
    } /// OK

    hash_map& operator=(const hash_map &hm) {
        clear();
        copy_constructor_wrapper(hm);
        return *this;
    } /// OK

    hash_map& operator=(hash_map &&hm) {
        swap(hm);
        hm.clear();
        return *this;
    } /// OK

    hash_map& operator=(std::initializer_list<value_type> l) {
        clear();
        for (auto el : l) {
            insert(el);
        }
        return *this;
    } /// OK

    allocator_type get_allocator() const noexcept {
        return Allocator;
    } /// OK

    bool empty() const noexcept {
        return Size == 0;
    } /// OK

    size_type size() const noexcept {
        return Size;
    } /// OK

    size_type max_size() const noexcept {
        return Capacity;
    } /// OK

    iterator begin() noexcept {
        int i = -1;
        while (Info[++i].status != PLACED && Info[i].status != END);
        return iterator(Info[i]);
    } /// OK

    const_iterator begin() const noexcept {
        int i = -1;
        while (Info[++i].status != PLACED && Info[i].status != END);
        return const_iterator(Info[i]);
    } /// OK

    const_iterator cbegin() const noexcept {
        return begin();
    } /// OK

    iterator end() noexcept {
        return iterator(Info[Capacity]);
    } /// OK

    const_iterator end() const noexcept {
        return const_iterator(Info[Capacity]);
    } /// OK

    const_iterator cend() const noexcept {
        return end();
    } /// OK

    template<typename... _Args>
    std::pair<iterator, bool> emplace(_Args&&... args) {
        return insert(value_type(std::forward<_Args&&>(args)...));
    } /// OK

    template <typename... _Args>
    std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args) {
        auto it = find(k);
        if (it == end()) {
            return std::pair<iterator, bool>(it, 0);
        }
        else {
            return insert({ k, std::forward<_Args&&>(args)... });
        }
    } /// OK

    template <typename... _Args>
    std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args) {
        auto it = find(k);
        if (it == end()) {
            return std::pair<iterator, bool>(it, 0);
        }
        else {
            return insert({ k, std::forward<_Args&&>(args)... });
        }
    } /// OK

    int calculate_hash(const key_type &key, const int &i) const {
        int hash = hash_function()(key);
        return (abs(hash) + (i * i)) % Capacity;
    } /// OK 1

    bool can_insert(const value_type& pair, const int &hash) {
        return (Info[hash].status == EMPTY || Info[hash].status == REMOVED) ||
            (Info[hash].status == PLACED && Data[hash].first == pair.first);
    } /// OK 1

    bool easy_insert(const value_type& pair, const int &hash) {
        if (Info[hash].status == PLACED) {
            Data[hash].second = pair.second;
            return false;
        }
        else {
            new (&Data[hash]) value_type(pair);
            Info[hash].status = PLACED;
            Size++;
            return true;
        }
    } /// OK 1

    std::pair<iterator, bool> insert(const value_type& x) {
        if ((double)(Size + 1) / Capacity >= max_load_factor()) {
            reserve(2 * (Size + 1));
        }
        int hash;
        bool unique = true;
        for (int i = 0; i < Capacity; i++) {
            hash = calculate_hash(x.first, i);
            if (can_insert(x, hash)) {
                unique = easy_insert(x, hash);
                break;
            }
        }
        return std::pair<iterator, bool>(iterator(Info[hash]), unique);
    } /// OK

    std::pair<iterator, bool> insert(value_type&& x) {
        return insert(x);
    } /// OK

    template<typename _InputIterator>
    void insert(_InputIterator first, _InputIterator last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    } /// OK

    void insert(std::initializer_list<value_type> l) {
        for (auto el : l) {
            insert(el);
        }
    } /// OK

    template <typename _Obj>
    std::pair<iterator, bool> insert_or_assign(const key_type& k, _Obj&& obj) {
        return insert(value_type(k, std::forward<mapped_type>(obj)));
    } /// OK

    template <typename _Obj>
    std::pair<iterator, bool> insert_or_assign(key_type&& k, _Obj&& obj) {
        return insert(value_type(k, std::forward<mapped_type>(obj)));
    } /// OK

    iterator erase(const_iterator position) {
        int index = position.operator->() - Data;
        position.operator->()->~value_type();
        Info[index].status = REMOVED;
        Size--;
        return iterator(Info[index+1]);
    } /// OK

    iterator erase(iterator position) {
        int index = position.operator->() - Data;
        position.operator->()->~value_type();
        Info[index].status = REMOVED;
        Size--;
        return iterator(Info[index + 1]);
    } /// OK

    size_type erase(const key_type& x) {
        auto it = find(x);
        if (it == end()) {
            return 0;
        }
        else {
            erase(it);
            return 1;
        }
    } /// OK

    iterator erase(const_iterator first, const_iterator last) {
        int index = last.operator->() - Data;
        for (; first != last; ++first) {
            erase(first);
        }
        return iterator(Info[index]);
    } /// OK

    void clear() noexcept {
        if (Size > 0) {
            erase(cbegin(), cend());
        }
    } /// OK

    void swap_move_wrapper(hash_map &x) {
        using std::swap;
        swap(this->LoadFactor, x.LoadFactor);
        swap(this->Capacity, x.Capacity);
        swap(this->Size, x.Size);
        swap(this->Data, x.Data);
        swap(this->Allocator, x.Allocator);
        swap(this->Hash_, x.Hash_);
        swap(this->Pred_, x.Pred_);
    }

    void swap(hash_map& x) noexcept {
        swap_move_wrapper(x);
        std::swap(this->Info, x.Info);
    } /// OK

    void move(hash_map &x) noexcept {
        swap_move_wrapper(x);
        this->Info = std::move(x.Info);
    } /// 

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>& source) {
        reserve(Size + source.Size);
        for (auto el : source) {
            insert(el);
        }
    } /// OK

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>&& source) {
        merge(source);
    } /// OK

    Hash hash_function() const {
        return Hash_;
    } /// OK

    Pred key_eq() const {
        return Pred_;
    } /// OK

    iterator find(const key_type& x) {
        int hash;
        for (int i = 0; i < Capacity; i++) {
            hash = calculate_hash(x, i);
            if (Info[hash].status == EMPTY) {
                return end();
            }
            else if (Info[hash].status == PLACED && Data[hash].first == x) {
                return iterator(Info[hash]);
            }
        }
        return end();
    } /// OK

    const_iterator find(const key_type& x) const {
        int hash;
        for (int i = 0; i < Capacity; i++) {
            hash = calculate_hash(x, i);
            if (Info[hash].status == EMPTY) {
                return end();
            }
            else if (Info[hash].status == PLACED && Data[hash].first == x) {
                return const_iterator(Info[hash]);
            }
        }
        return end();
    } /// OK

    size_type count(const key_type& x) const {
        return find(x) != end();
    } /// OK

    bool contains(const key_type& x) const {
        return find(x) != end();
    } /// OK

    mapped_type& operator[](const key_type& k) {
        auto it = find(k);
        if (it == end()) {
            auto elem = insert({ k, mapped_type() });
            return (*(elem.first)).second;
        }
        else {
            return (*it).second;
        }
    } /// OK

    mapped_type& operator[](key_type&& k) {
        return operator[](k);
    } /// OK

    mapped_type& at(const key_type& k) {
        auto it = find(k);
        if (it == end()) {
            throw std::out_of_range("");
        }
        else {
            return (*it).second;
        }
    } /// OK

    const mapped_type& at(const key_type& k) const {
        auto it = find(k);
        if (it == end()) {
            throw std::out_of_range("");
        }
        else {
            return (*it).second;
        }
    } /// OK

    size_type bucket_count() const noexcept {
        return Size;
    } /// OK

    size_type bucket(const key_type& x) const {
        auto it = find(x);
        return (it.operator->() - Data);
    } /// OK

    float load_factor() const noexcept {
        return (float)Size / Capacity;
    } /// OK

    float max_load_factor() const noexcept {
        return LoadFactor;
    } /// OK

    void max_load_factor(float z) {
        float ratio = z / max_load_factor();
        LoadFactor = z;
        rehash(ceil(Capacity / ratio));
    } /// OK

    void rehash(size_type Capacity_) {
        if ((double)(Size / Capacity_) <= max_load_factor()) {
            // ===== temporary objects
            int tmp_Capacity = Capacity;
            value_type *tmp_Data = std::move(Data);
            std::vector<Node<value_type>> tmp_Info = std::move(Info);
            // ===== allocating memory
            Info = std::vector<Node<value_type>>(Capacity_ + 1);
            Data = Allocator.allocate(Capacity_ + 1);
            Capacity = Capacity_;
            Size = 0; // don't touch this !!!
            // ===== link 
            for (int i = 0; i < Capacity_; i++) {
                Info[i] = Node<value_type>(&Data[i], EMPTY);
            }
            Info[Capacity_] = Node<value_type>(&Data[Capacity_], END);
            // ===== push data
            int i, hash;
            for (Node<value_type> node : tmp_Info) {
                if (node.status == PLACED) {
                    for (i = 0; i < Capacity_; i++) {
                        hash = calculate_hash((*node)->first, i);
                        if (can_insert(**node, hash)) {
                            new (&Data[hash]) value_type(**node);
                            if (Info[hash].status != PLACED) {
                                Info[hash].status = PLACED;
                                Size++;
                            }
                            break;
                        }
                    }
                }
            }
            // ===== deallocating memory
            Allocator.deallocate(tmp_Data, tmp_Capacity + 1);
            tmp_Info.clear();
        }
    } /// OK

    void reserve(size_type n) {
        rehash(ceil(n / LoadFactor));
    } /// OK

    bool operator==(const hash_map& other) const {
        if (Size != other.Size) {
            return false;
        }
        else {
            std::vector<K> vec1, vec2;
            for (auto el : *this) {
                vec1.push_back(el.first);
            }
            for (auto el : other) {
                vec2.push_back(el.first);
            }
            sort(vec1.begin(), vec1.end());
            sort(vec2.begin(), vec2.end());
            return vec1 == vec2;
        }
    } /// OK

private:
    // =====
    float LoadFactor;
    int Capacity;
    int Size;
    // =====
    std::vector<Node<value_type>> Info;
    value_type *Data;
    Alloc Allocator;
    // =====
    Hash Hash_;
    Pred Pred_;
};

} // namespace fefu
