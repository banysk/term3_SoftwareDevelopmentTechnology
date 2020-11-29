#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace fefu
{

template<typename T>
class allocator {
public:
    // naming tradition
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
    allocator(const allocator<U>&) noexcept {}
    ~allocator() = default;
    pointer allocate(size_type size) {
        return static_cast<T*>(::operator new(size * sizeof(T)));
    }
    void deallocate(T* p, size_t n) noexcept {
        if (static_cast<void*>(p) != nullptr) {
            ::operator delete(static_cast<void*>(p), n * sizeof(T));
        }
    }
};

template<typename ValueType>
class hash_map_iterator {
public:
    // naming tradition
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType&;
    using pointer = ValueType*;
    // methods
    hash_map_iterator() noexcept = default;
    hash_map_iterator(ValueType* ptr) {
        this->ptr = ptr;
    }
    hash_map_iterator(const hash_map_iterator& other) noexcept {
        this->ptr = other.ptr;
    }
    reference operator*() const {
        return *ptr;
    }
    pointer operator->() const {
        return ptr;
    }
    hash_map_iterator& operator++() {
        this->ptr++;
        return *this;
    }
    hash_map_iterator operator++(int) {
        hash_map_iterator<ValueType> this_copy = *this;
        this->ptr++;
        return this_copy;
    }
    friend bool operator==(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return static_cast<void*>(l.ptr) == static_cast<void*>(r.ptr);
    }
    friend bool operator!=(const hash_map_iterator<ValueType> &l, const hash_map_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // fields
    ValueType* ptr;
};

template<typename ValueType>
class hash_map_const_iterator {
// Shouldn't give non const references on value
public:
    // naming tradition
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType&;
    using pointer = const ValueType*;
    // methods
    hash_map_const_iterator() noexcept = default;
    hash_map_const_iterator(ValueType* ptr) {
        this->ptr = ptr;
    }
    hash_map_const_iterator(const hash_map_const_iterator& other) noexcept {
        this->ptr = other.ptr;
    };
    hash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept {
        this->ptr = other.ptr;
    };
    reference operator*() const {
        return *ptr;
    }
    pointer operator->() const {
        return ptr;
    }
    hash_map_const_iterator& operator++() {
        this->ptr++;
        return *this;
    }
    hash_map_const_iterator operator++(int) {
        hash_map_const_iterator<ValueType> this_copy = *this;
        this->ptr++;
        return this_copy;
    }
    friend bool operator==(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return static_cast<void*>(l.ptr) == static_cast<void*>(r.ptr);
    }
    friend bool operator!=(const hash_map_const_iterator<ValueType> &l, const hash_map_const_iterator<ValueType> &r) {
        return !(l == r);
    }

private:
    // fields
    ValueType* ptr;
};

template<typename K, typename T,
	   typename Hash = std::hash<K>,
	   typename Pred = std::equal_to<K>,
	   typename Alloc = allocator<std::pair<const K, T>>>
class hash_map
{
public:
    // naming tradition
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
    // constructors and destructors
    hash_map() { // OK, TESTED
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = 0;
        this->load_factor_ = (float)0.9;
    }
    explicit hash_map(size_type n) { // OK, TESTED
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = n;
        this->load_factor_ = (float)0.9;
        if (this->capacity_ > 0) {
            this->data_ = allocator_.allocate(this->capacity_);
            this->is_placed_ = bool_allocator_.allocate(this->capacity_);
            this->is_removed_ = bool_allocator_.allocate(this->capacity_);
            for (int i = 0; i < this->capacity_; i++) {
                is_placed_[i] = false;
                is_removed_[i] = false;
            }
        }
    }
    ~hash_map() { // OK, TESTED
        this->allocator_.deallocate(this->data_, this->capacity_);
        this->bool_allocator_.deallocate(this->is_placed_, this->capacity_);
        this->bool_allocator_.deallocate(this->is_removed_, this->capacity_);
    }
    template<typename InputIterator> // OK, TESTED
    hash_map(InputIterator first, InputIterator last, size_type n = 0) { // 
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = 0;
        this->load_factor_ = (float)0.9;
        int len = last.operator->() - first.operator->();
        int new_capacity = std::max<int>(n, len);
        if (new_capacity > 0) {
            reserve(new_capacity);
            for (auto it_ = first; it_.operator->() < last.operator->(); it_++) {
                insert(*it_);
            }
        }
    }
    hash_map(const hash_map &hm) { // OK, TESTED
        this->allocator_ = hm.allocator_;
        this->bool_allocator_ = hm.bool_allocator_;
        this->size_ = hm.size_;
        this->capacity_ = hm.capacity_;
        this->load_factor_ = hm.load_factor_;
        this->data_ = this->allocator_.allocate(this->capacity_);
        memcpy(this->data_, hm.data_, this->capacity_ * sizeof(value_type));
        this->is_placed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_placed_, hm.is_placed_, this->capacity_ * sizeof(bool));
        this->is_removed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_removed_, hm.is_removed_, this->capacity_ * sizeof(bool));
    }
    hash_map(hash_map &&hm) { // OK, TESTED
        this->size_ = hm.size_;
        hm.size_ = 0;
        this->capacity_ = hm.capacity_;
        hm.capacity_ = 0;
        this->load_factor_ = hm.load_factor_;
        this->data_ = hm.data_;
        hm.data_ = nullptr;
        this->is_placed_ = hm.is_placed_;
        hm.is_placed_ = nullptr;
        this->is_removed_ = hm.is_removed_;
        hm.is_removed_ = nullptr;
        this->allocator_ = hm.allocator_;
        this->bool_allocator_ = hm.bool_allocator_;
    }
    explicit hash_map(const allocator_type& a) { // OK, TESTED
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = 0;
        this->load_factor_ = (float)0.9;
        this->allocator_ = a;
    }
    hash_map(const hash_map& hm, const allocator_type& a) { // OK, TESTED
        this->allocator_ = a;
        this->bool_allocator_ = hm.bool_allocator_;
        this->size_ = hm.size_;
        this->capacity_ = hm.capacity_;
        this->load_factor_ = hm.load_factor_;
        this->data_ = this->allocator_.allocate(this->capacity_);
        memcpy(this->data_, hm.data_, this->capacity_ * sizeof(value_type));
        this->is_placed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_placed_, hm.is_placed_, this->capacity_ * sizeof(bool));
        this->is_removed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_removed_, hm.is_removed_, this->capacity_ * sizeof(bool));
    }
    hash_map(hash_map&& hm, const allocator_type& a) { // OK, TESTED
        this->size_ = hm.size_;
        hm.size_ = 0;
        this->capacity_ = hm.capacity_;
        hm.capacity_ = 0;
        this->load_factor_ = hm.load_factor_;
        this->data_ = hm.data_;
        hm.data_ = nullptr;
        this->is_placed_ = hm.is_placed_;
        hm.is_placed_ = nullptr;
        this->is_removed_ = hm.is_removed_;
        hm.is_removed_ = nullptr;
        this->allocator_ = a;
        this->bool_allocator_ = hm.bool_allocator_;
    }
    hash_map(std::initializer_list<value_type> l, size_type n = 0) { // OK, TESTED
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = 0;
        this->load_factor_ = (float)0.9;
        int len = l.size();
        int new_capacity = std::max<int>(n, len);
        if (new_capacity > 0) {
            reserve(new_capacity);
            for (auto n : l) {
                insert(n);
            }
        }
    }
    hash_map& operator=(const hash_map &hm) { // OK, TESTED
        this->allocator_ = hm.allocator_;
        this->bool_allocator_ = hm.bool_allocator_;
        this->size_ = hm.size_;
        this->capacity_ = hm.capacity_;
        this->load_factor_ = hm.load_factor_;
        this->data_ = this->allocator_.allocate(this->capacity_);
        memcpy(this->data_, hm.data_, this->capacity_ * sizeof(value_type));
        this->is_placed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_placed_, hm.is_placed_, this->capacity_ * sizeof(bool));
        this->is_removed_ = this->bool_allocator_.allocate(this->capacity_);
        memcpy(this->is_removed_, hm.is_removed_, this->capacity_ * sizeof(bool));
        return *this;
    }
    hash_map& operator=(hash_map &&hm) { // OK, TESTED
        this->size_ = hm.size_;
        hm.size_ = 0;
        this->capacity_ = hm.capacity_;
        hm.capacity_ = 0;
        this->load_factor_ = hm.load_factor_;
        this->data_ = hm.data_;
        hm.data_ = nullptr;
        this->is_placed_ = hm.is_placed_;
        hm.is_placed_ = nullptr;
        this->is_removed_ = hm.is_removed_;
        hm.is_removed_ = nullptr;
        this->allocator_ = hm.allocator_;
        this->bool_allocator_ = hm.bool_allocator_;
        return *this;
    }
    hash_map& operator=(std::initializer_list<value_type> l) { // OK, TESTED
        this->data_ = nullptr;
        this->size_ = 0;
        this->capacity_ = 0;
        this->load_factor_ = (float)0.9;
        for (auto n : l) {
            insert(n);
        }
        return *this;
    }
    allocator_type get_allocator() const noexcept { // OK, TESTED
        return this->allocator_;
    }
    // size and capacity.
    bool empty() const noexcept { // OK, TESTED
        return this->size_ == 0;
    }
    size_type size() const noexcept { // OK, TESTED
        return this->size_;
    }
    size_type max_size() const noexcept { // OK, TESTED
        return this->capacity_;
    }
    // iterators.
    iterator begin() noexcept { // OK, TESTED
        return hash_map_iterator<std::pair<const K, T>>(this->data_);
    }
    const_iterator begin() const noexcept { // OK, TESTED
        return hash_map_const_iterator<std::pair<const K, T>>(this->data_);
    }
    const_iterator cbegin() const noexcept { // OK, TESTED
        return begin();
    }
    iterator end() noexcept { // OK, TESTED
        if (this->data_ == nullptr) {
            return hash_map_iterator<std::pair<const K, T>>(nullptr);
        }
        else {
            return hash_map_iterator<std::pair<const K, T>>(this->data_ + (this->capacity_ - 1));
        }
    }
    const_iterator end() const noexcept { // OK, TESTED
        if (this->data_ == nullptr) {
            return hash_map_const_iterator<std::pair<const K, T>>(nullptr);
        }
        else {
            return hash_map_const_iterator<std::pair<const K, T>>(this->data_ + (this->capacity_ - 1));
        }
    }
    const_iterator cend() const noexcept { // OK, TESTED
        return end();
    }
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

    std::pair<iterator, bool> insert(const value_type& x) { // OK, TESTED
        if ((float)(this->size_ + 1)/this->capacity_ >= this->load_factor_) {
            rehash(ceil(((float)this->size_ + 1) / this->load_factor_));
        }
        int hash = std::hash<K>()(x.first);
        hash = abs(hash);
        int i;
        bool unique = true;
        for (i = 0; i < this->capacity_; i++) {
            if (this->is_placed_[(hash + i) % this->capacity_]) {
                if (x.first == this->data_[(hash + i) % this->capacity_].first) {
                    unique = false;
                    continue;
                }
            }
            if (!this->is_placed_[(hash + i) % this->capacity_]) {
                this->is_placed_[(hash + i) % this->capacity_] = true;
                this->is_removed_[(hash + i) % this->capacity_] = false;
                this->size_++;
                break;
            }
        }
        memcpy(&this->data_[(hash + i) % this->capacity_], &x, sizeof(x));
        return std::make_pair<iterator, bool>(iterator(&this->data_[(hash + i) % this->capacity_]), (int)unique);
    }
    std::pair<iterator, bool> insert(value_type&& x) { // OK, TESTED
        return insert(x);
    }
    template<typename _InputIterator>
    void insert(_InputIterator first, _InputIterator last) { // OK, TESTED
        for (auto it_ = first; it_.operator->() < last.operator->(); it_++) {
            insert(*it_);
        }
    }
    void insert(std::initializer_list<value_type> l) { // OK, TESTED
        for (auto n : l) {
            insert(n);
        }
    }


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
        int place = position.operator->() - this->data_;
        this->size_--;
        this->is_placed_[place] = false;
        this->is_removed_[place] = true;
        if (place > 0 && this->is_placed_[place - 1]) {
            return iterator(&this->data_[place - 1]);
        }
        else {
            return end();
        }
    }
    iterator erase(iterator position) {
        int place = position.operator->() - this->data_;
        this->size_--;
        this->is_placed_[place] = false;
        this->is_removed_[place] = true;
        if (place > 0 && this->is_placed_[place - 1]) {
            return iterator(&this->data_[place - 1]);
        }
        else {
            return end();
        }
    }
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

    void clear() noexcept {
        for (int i = 0; i < this->capacity_; i++) {
            this->is_placed_[i] = false;
            this->is_removed_[i] = false;
        }
        if (this->data_ != nullptr) {
            this->allocator_.deallocate(this->data_, this->capacity_);
        }
        this->size_ = 0;
    }

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
    void merge(hash_map<K, T, _H2, _P2, Alloc>& source);

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc>&& source);

    // observers.
    Hash hash_function() const {
        return Hash;
    }
    Pred key_eq() const {
        return Pred;
    }

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
    //@}

    // bucket interface.
    size_type bucket_count() const noexcept { // OK, TESTED
        return this->size();
    }

    /*
    * @brief  Returns the bucket index of a given element.
    * @param  _K  A key instance.
    * @return  The key bucket index.
    */
    size_type bucket(const key_type& _K) const;

    // hash policy.
    float load_factor() const noexcept { // OK, TESTEd
        if (this->capacity_ == 0) {
            return 0;
        }
        else {
            return (float)this->size_ / this->capacity_;

        }
    }
    float max_load_factor() const noexcept { // OK, TESTED
        return this->load_factor_;
    }
    void max_load_factor(float z) { // OK, TESTED
        float tmp = z / this->load_factor_;
        this->load_factor_ = z;
        rehash(ceil(this->capacity_ / tmp));
    }
    void rehash(size_type n) { // OK, TESTED
        if ((this->size_ / (float)n < this->load_factor_)) {
            std::pair<const K, T> *tmp_data = this->data_;
            this->data_ = this->allocator_.allocate(n);
            bool *tmp_is_placed = this->is_placed_;
            this->is_placed_ = this->bool_allocator_.allocate(n);
            this->is_removed_ = this->bool_allocator_.allocate(n);
            for (int i = 0; i < n; i++) {
                is_placed_[i] = false;
                is_removed_[i] = false;
            }
            int tmp_capacity = this->capacity_;
            this->capacity_ = n;
            this->size_ = 0;
            for (int i = 0; i < tmp_capacity; i++) {
                if (tmp_is_placed[i]) {
                    insert({ tmp_data[i].first, tmp_data[i].second });
                }
            }
        }
    }
    void reserve(size_type n) { // OK, TESTED
        if (n > 0) {
            rehash(ceil((float)n / this->load_factor_));
        }
    }

    bool operator==(const hash_map& other) const;
private:
    // fields
    std::pair<const K, T> *data_;
    bool *is_placed_;
    bool *is_removed_;
    int size_;
    int capacity_;
    float load_factor_;
    allocator<std::pair<const K, T>> allocator_;
    allocator<bool> bool_allocator_;
};

} // namespace fefu
