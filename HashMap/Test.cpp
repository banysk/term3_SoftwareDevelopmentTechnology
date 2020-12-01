#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hash_map.hpp"
#include <fstream>
#include <vector>

using namespace fefu;

TEST_CASE("Test") {
    SECTION("allocator") {
        allocator<int> a;
        int* var = a.allocate(10);
        a.deallocate(var, 10);
    }

    SECTION("hash_map", "[hash_map()]") {
        hash_map<std::string, int> hm;
        REQUIRE(hm.size() == 0);
        REQUIRE(hm.max_size() == 0);
        REQUIRE(hm.load_factor() == 0.0f);
        REQUIRE(hm.empty() == true);
        hm.hash_function();
        hm.key_eq();
    }

    SECTION("hash_map", "[explicit hash_map(size_type n)]") {
        hash_map<std::string, int> hm(10);
        REQUIRE(hm.bucket_count() == 0);
        REQUIRE(hm.max_size() == 10);
        REQUIRE(hm.load_factor() == 0.0f);
    }

    SECTION("hash_map", "[hash_map(InputIterator first, InputIterator last, size_type n = 0)]") {
        hash_map<std::string, int> from(10);
        for (int i = 0; i < 5; i++) {
            from.insert({std::to_string(i), i});
        }
        auto b = from.begin(), e = from.end();
        hash_map<std::string, int> to(b, e, 5);
        REQUIRE(to.size() == 5);
    }

    SECTION("hash_map", "[hash_map(const hash_map &hm)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm1({ a, b, c, d, e }, 5);
        hash_map<std::string, int> hm2(hm1);
        REQUIRE(hm1.size() == hm2.size());
    }

    SECTION("hash_map", "[hash_map(hash_map &&hm)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm = move(hash_map<std::string, int>({a, b, c, d, e}));
        REQUIRE(hm.size() == 5);
    }

    SECTION("hash_map", "[explicit hash_map(const allocator_type& a)]") {
        allocator<std::pair<const std::string, int>> a, b;
        hash_map<std::string, int> hm(a);
        b = hm.get_allocator();
        REQUIRE(typeid(a).name() == typeid(b).name());
    }

    SECTION("hash_map", "[hash_map(const hash_map& umap, const allocator_type& a)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm1({ a, b, c, d, e }, 5);
        allocator<std::pair<const std::string, int>> alloc;
        hash_map<std::string, int> hm2(hm1, alloc);
        REQUIRE(hm1.size() == hm2.size());
    }

    SECTION("hash_map", "[hash_map(hash_map&& umap, const allocator_type& a)]") {
        //std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        //allocator<std::pair<const std::string, int>> alloc;
        //hash_map<std::string, int> hm = move(hash_map<std::string, int>({ a, b, c, d, e }), alloc);
        //REQUIRE(hm.size() == 5);
    }

    SECTION("hash_map", "[hash_map(std::initializer_list<value_type> l, size_type n = 0)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm({ a, b, c, d, e }, 5);
        REQUIRE(hm.size() == 5);
    }

    SECTION("hash_map", "[hash_map& operator=(const hash_map&)") {

    }

    SECTION("hash_map", "[hash_map& operator=(hash_map&&)") {

    }

    SECTION("hash_map", "[hash_map& operator=(std::initializer_list<value_type> l)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm;
        hm = {a, b, c, d, e};
        REQUIRE(hm.size() == 5);
    }

    SECTION("hash_map", "[std::pair<iterator, bool> emplace(_Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> try_emplace(const key_type& k, _Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> try_emplace(key_type&& k, _Args&&... args)]") {

    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert(const value_type& x)]") {
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm;
        for (int i = 0; i < 10; i++) {
            std::pair<const std::string, int> pair("1", i);
            _pair p = hm.insert(pair);
            REQUIRE(p.first->first == "1");
            REQUIRE(p.first->second == i);
            if (i == 0) {
                REQUIRE(p.second == true);
            }
            else {
                REQUIRE(p.second == false);
            }
        }
        hash_map<std::string, int> hm1(1);
        std::pair<const std::string, int> pair("1", 1);
        hm1.insert(pair);
    }

    SECTION("hash_map", "[std::pair<iterator, bool> insert(value_type&& x)]") {
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm;
        for (int i = 0; i < 10; i++) {
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
        hash_map<std::string, int> from(10);
        for (int i = 0; i < 5; i++) {
            from.insert({ std::to_string(i), i });
        }
        auto b = from.begin(), e = from.end();
        hash_map<std::string, int> to;
        to.insert(b, e);
        REQUIRE(to.size() == 5);
    }

    SECTION("hash_map", "[void insert(std::initializer_list<value_type> l)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm;
        hm.insert({a, b, c, d, e});
        REQUIRE(hm.size() == 5);
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

    SECTION("hash_map", "[size_type bucket(const key_type& _K) const]") {

    }

    SECTION("hash_map", "[void max_load_factor(float z)]") {
        hash_map<std::string, int> hm;
        hm.reserve(100);
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
        }
        hm.max_load_factor(0.1f);
        REQUIRE(hm.max_load_factor() == 0.1f);
        REQUIRE(hm.load_factor() == 0.1f);
        REQUIRE(hm.size() == 100);
        REQUIRE(hm.max_size() == 1000);

        std::ofstream fout("out.txt");
        for (auto it : hm) {
            fout << "\"" << it.first << "\":=" << it.second << "\n";
        }

        hash_map_const_iterator<std::pair<const std::string, int>> b, e;
        b = hm.cbegin();
        e = hm.cend();
        fout << "====================\n";
        for (; b != e; ++b) {
            fout << "\"" << (*b).first << "\":=" << (*b).second << "\n";
        }
    }

    SECTION("hash_map", "[void rehash(size_type n)]") {
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm;
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
            REQUIRE(hm.size() == i + 1);
            REQUIRE(hm.load_factor() <= hm.max_load_factor());
        }
    }

    SECTION("hash_map", "[void reserve(size_type n)]") {
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm;
        hm.reserve(100);
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
            REQUIRE(hm.max_size() == (float)100/hm.max_load_factor());
            REQUIRE(hm.load_factor() <= hm.max_load_factor());
        }
    }

    SECTION("hash_map", "[bool operator==(const hash_map& other) const]") {

    }
}