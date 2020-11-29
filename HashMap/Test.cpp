#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hash_map.hpp"
#include <fstream>
#include <vector>

using namespace fefu;

TEST_CASE("Test") {
    std::vector<std::string> data;

    SECTION("allocator") {
        allocator<int> a;
        int* var = a.allocate(10);
        a.deallocate(var, 10);
    }

    SECTION("hash_map_iterator") {
        Node<std::pair<const std::string, int>> node;
        std::pair<const std::string, int> pair("test", 1);
        node.PtrToValue = &pair;
        hash_map_iterator<std::pair<const std::string, int>> it1(node), it2(node);
        REQUIRE(it1 == it2);
        REQUIRE(*it1 == pair);
        REQUIRE(*it2 == pair);
        REQUIRE(it1.operator->() == &pair);
        REQUIRE(it2.operator->() == &pair);
        REQUIRE(it1++ == it2++);
        REQUIRE(++it1 == ++it2);
        it1++;
        REQUIRE(it1 == ++it2);
    }

    SECTION("hash_map_const_iterator") {
        Node<std::pair<const std::string, int>> node;
        std::pair<const std::string, int> pair("test", 1);
        node.PtrToValue = &pair;
        hash_map_iterator<std::pair<const std::string, int>> it1(node);
        hash_map_const_iterator<std::pair<const std::string, int>> it2(it1), it3(it1);
        REQUIRE(it2 == it2);
        REQUIRE(*it2 == pair);
        REQUIRE(*it3 == pair);
        REQUIRE(it2.operator->() == &pair);
        REQUIRE(it3.operator->() == &pair);
        REQUIRE(it2++ == it3++);
        REQUIRE(++it2 == ++it3);
        it2++;
        REQUIRE(it2 == ++it3);
    }

    SECTION("hash_map", "[hash_map()]") {
        hash_map<std::string, int> hm;
    }

    SECTION("hash_map", "[explicit hash_map(size_type n)]") {
        hash_map<std::string, int> hm(10);
    }

    SECTION("hash_map", "[hash_map(InputIterator first, InputIterator last, size_type n = 0)]") {

    }

    SECTION("hash_map", "[hash_map(const hash_map &hm)]") {

    }

    SECTION("hash_map", "[hash_map(hash_map &&hm)]") {

    }

    SECTION("hash_map", "[explicit hash_map(const allocator_type& a)]") {

    }

    SECTION("hash_map", "[hash_map(const hash_map& umap, const allocator_type& a)]") {

    }

    SECTION("hash_map", "[hash_map(hash_map&& umap, const allocator_type& a)]") {

    }

    SECTION("hash_map", "[hash_map(std::initializer_list<value_type> l, size_type n = 0)]") {

    }

    SECTION("hash_map", "[hash_map& operator=(const hash_map&)") {

    }

    SECTION("hash_map", "[hash_map& operator=(hash_map&&)") {

    }

    SECTION("hash_map", "[hash_map& operator=(std::initializer_list<value_type> l)]") {

    }

    SECTION("hash_map", "[allocator_type get_allocator() const noexcept]") {

    }

    SECTION("hash_map", "[bool empty() const noexcept]") {

    }

    SECTION("hash_map", "[size_type size() const noexcept]") {

    }

    SECTION("hash_map", "[size_type max_size() const noexcept]") {

    }

    SECTION("hash_map", "[iterator begin() noexcept]") {

    }

    SECTION("hash_map", "[const_iterator begin() const noexcept]") {

    }

    SECTION("hash_map", "[const_iterator cbegin() const noexcept]") {

    }

    SECTION("hash_map", "[iterator end() noexcept]") {

    }

    SECTION("hash_map", "[const_iterator end() const noexcept]") {

    }

    SECTION("hash_map", "[const_iterator cend() const noexcept]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> emplace(_Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert(const value_type& x)]") {
        
    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert(value_type&& x)]") {
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm(1);
        for (int i = 0; i < 100; i++) {
            _pair p = hm.insert({ "1", i });
            REQUIRE(p.first->first == "1");
            REQUIRE(p.first->second == i);
            if (i == 0) {
                REQUIRE(p.second == true);
            }
            else {
                REQUIRE(p.second == false);
            }
        }
    }

    SECTION("hash_map", "[void insert(_InputIterator first, _InputIterator last)]") {

    }

    SECTION("hash_map", "[void insert(std::initializer_list<value_type> l)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert_or_assign(const key_type& k, _Obj&& obj)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert_or_assign(key_type&& k, _Obj&& obj)]") {

    }

    SECTION("hash_map", "[iterator erase(const_iterator position)]") {

    }

    SECTION("hash_map", "[iterator erase(iterator position)]") {

    }

    SECTION("hash_map", "[size_type erase(const key_type& x)]") {

    }

    SECTION("hash_map", "[iterator erase(const_iterator first, const_iterator last)]") {

    }

    SECTION("hash_map", "[void clear() noexcept]") {

    }

    SECTION("hash_map", "[void swap(hash_map& x)]") {

    }

    SECTION("hash_map", "[void merge(hash_map<K, T, _H2, _P2, Alloc>& source)]") {

    }

    SECTION("hash_map", "[void merge(hash_map<K, T, _H2, _P2, Alloc>&& source)]") {

    }

    SECTION("hash_map", "[Hash hash_function() const]") {

    }

    SECTION("hash_map", "[Pred key_eq() const]") {

    }

    SECTION("hash_map", "[iterator find(const key_type& x)]") {

    }

    SECTION("hash_map", "[const_iterator find(const key_type& x) const]") {

    }

    SECTION("hash_map", "[size_type count(const key_type& x) const]") {

    }

    SECTION("hash_map", "[bool contains(const key_type& x) const]") {

    }

    SECTION("hash_map", "[mapped_type& operator[](const key_type& k)]") {

    }

    SECTION("hash_map", "[mapped_type& operator[](key_type&& k)]") {

    }

    SECTION("hash_map", "[mapped_type& at(const key_type& k)]") {

    }

    SECTION("hash_map", "[const mapped_type& at(const key_type& k) const]") {

    }

    SECTION("hash_map", "[size_type bucket_count() const noexcept]") {

    }

    SECTION("hash_map", "[size_type bucket(const key_type& _K) const]") {

    }

    SECTION("hash_map", "[float load_factor() const noexcept]") {
  
    }

    SECTION("hash_map", "[float max_load_factor() const noexcept]") {

    }

    SECTION("hash_map", "[void max_load_factor(float z)]") {

    }

    SECTION("hash_map", "[void rehash(size_type n)]") {

    }

    SECTION("hash_map", "[void reserve(size_type n)]") {

    }

    SECTION("hash_map", "[bool operator==(const hash_map& other) const]") {

    }
}