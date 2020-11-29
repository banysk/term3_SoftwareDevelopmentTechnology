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
        Node<std::pair<std::string, int>> node;
        std::pair<std::string, int> pair("test", 1);
        node.ptr = &pair;
        node.status = PLACED;
        hash_map_iterator<std::pair<std::string, int>> it1(node), it2(node);
        hash_map_iterator<std::pair<std::string, int>> it3(it2);
        REQUIRE(it2++ == it3++);
        REQUIRE(++it2 != it3++);
        REQUIRE(it1.operator->() != nullptr);
        REQUIRE(it1->first == "test");
        REQUIRE(it1->second == 1);
        REQUIRE(node.status == PLACED);
    }

    SECTION("hash_map_const_iterator") {
        Node<std::pair<std::string, int>> node;
        std::pair<std::string, int> pair("test", 1);
        node.ptr = &pair;
        node.status = PLACED;
        hash_map_const_iterator<std::pair<std::string, int>> it1(node), it2(node);
        hash_map_const_iterator<std::pair<std::string, int>> it3(it2);
        REQUIRE(it2++ == it3++);
        REQUIRE(++it2 != it3++);
        REQUIRE(it1.operator->() != nullptr);
        REQUIRE(it1->first == "test");
        REQUIRE(it1->second == 1);
        REQUIRE(node.status == PLACED);
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
        hash_map<std::string, int> hm1(10);
        hash_map<std::string, int> hm2(hm1);
    }

    SECTION("hash_map", "[hash_map(hash_map &&hm)]") {
        hash_map<std::string, int> hm1(10);
        hash_map<std::string, int> hm2 = move(hm1);
    }

    SECTION("hash_map", "[explicit hash_map(const allocator_type& a)]") {
        allocator<std::pair<const std::string, int>> a;
        hash_map<std::string, int> hm(a);
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
        allocator<std::pair<const std::string, int>> a;
        hash_map<std::string, int> hm(a);
        REQUIRE(typeid(hm.get_allocator()).name() == typeid(a).name());
    }

    SECTION("hash_map", "[bool empty() const noexcept]") {
        hash_map<std::string, int> hm;
        REQUIRE(hm.empty() == true);
    }

    SECTION("hash_map", "[size_type size() const noexcept]") {
        hash_map<std::string, int> hm;
        REQUIRE(hm.size() == 0);
    }

    SECTION("hash_map", "[size_type max_size() const noexcept]") {
        {
            hash_map<std::string, int> hm;
            REQUIRE(hm.max_size() == 0);
        }
        {
            hash_map<std::string, int> hm(10);
            REQUIRE(hm.max_size() == 10);
        }
    }

    SECTION("hash_map", "[iterator begin() noexcept]") {
        hash_map<std::string, int> hm(1);
        hash_map_iterator<std::pair<const std::string, int>> it;
        REQUIRE(hm.begin() != it);
    }

    SECTION("hash_map", "[const_iterator begin() const noexcept]") {

    }

    SECTION("hash_map", "[const_iterator cbegin() const noexcept]") {

    }

    SECTION("hash_map", "[iterator end() noexcept]") {
        hash_map<std::string, int> hm(1);
        hash_map_iterator<std::pair<const std::string, int>> it;
        REQUIRE(hm.end() != it);
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
        std::ofstream fout("output.txt", 'w');
        hash_map<std::string, int> hm;
        hm.max_load_factor(0.1);
        std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool> pair;
        pair = hm.insert({ "test", 1 });
        REQUIRE(pair.first->first == "test");
        REQUIRE(pair.first->second == 1);
        REQUIRE(pair.second == true);
        pair = hm.insert({ "test", 111 });
        REQUIRE(pair.first->first == "test");
        REQUIRE(pair.first->second == 111);
        REQUIRE(pair.second == false);
        for (int i = 0; i < 1000; i++) {
            hm.insert({ std::to_string(i), i });
            fout << hm.size() << "/" << hm.max_size() << " : " << hm.load_factor() << "\n";
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
        hash_map<std::string, int> hm;
        REQUIRE(typeid(hm.hash_function()).name() == typeid(std::hash<std::string>).name());
    }

    SECTION("hash_map", "[Pred key_eq() const]") {
        hash_map<std::string, int> hm;
        REQUIRE(typeid(hm.key_eq()).name() == typeid(std::equal_to<std::string>).name());
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
        hash_map<std::string, int> hm(10);
        REQUIRE(hm.bucket_count() == 10);
    }

    SECTION("hash_map", "[size_type bucket(const key_type& _K) const]") {

    }

    SECTION("hash_map", "[float load_factor() const noexcept]") {
        hash_map<std::string, int> hm;
        REQUIRE(hm.load_factor() == 0.0f);
    }

    SECTION("hash_map", "[float max_load_factor() const noexcept]") {
        hash_map<std::string, int> hm;
        REQUIRE(hm.max_load_factor() == 0.9f);
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