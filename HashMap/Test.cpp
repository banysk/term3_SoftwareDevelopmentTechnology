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
        node.Status = PLACED;
        hash_map_iterator<std::pair<const std::string, int>> it(node), it1(node), it2(node);
        // (it.operator->())->first = "new"; *ERROR, first is const
        // (it.operator->())->second = 10; *OK, second is non const
        (it.operator->())->second = 10; // *OK
        REQUIRE((*it).first == "test");
        REQUIRE((*it).second == 10);
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
        node.Status = PLACED;
        hash_map_iterator<std::pair<const std::string, int>> it1(node);
        hash_map_const_iterator<std::pair<const std::string, int>> it(node), it2(it1), it3(it1);
        // (it.operator->())->first = "new"; *ERROR, it is const
        // (it.operator->())->second = 10; *ERROR, it is const
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
        REQUIRE(hm.size() == 0);
        REQUIRE(hm.max_size() == 0);
        REQUIRE(hm.load_factor() == 0.0f);
        REQUIRE(hm.empty() == true);
    }

    SECTION("hash_map", "[explicit hash_map(size_type n)]") {
        hash_map<std::string, int> hm(10);
        REQUIRE(hm.bucket_count() == 0);
        REQUIRE(hm.max_size() == 10);
        REQUIRE(hm.load_factor() == 0.0f);
    }

    SECTION("hash_map", "[hash_map(InputIterator first, InputIterator last, size_type n = 0)]") {

    }

    SECTION("hash_map", "[hash_map(const hash_map &hm)]") {

    }

    SECTION("hash_map", "[hash_map(hash_map &&hm)]") {

    }

    SECTION("hash_map", "[explicit hash_map(const allocator_type& a)]") {
        allocator<std::pair<const std::string, int>> a, b;
        hash_map<std::string, int> hm(a);
        b = hm.get_allocator();
        REQUIRE(typeid(a).name() == typeid(b).name());
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

    SECTION("hash_map", "[iterators]") {
        hash_map<std::string, int> hm(10);
        hash_map_iterator<std::pair<const std::string, int>> begin, end;
        hash_map_const_iterator<std::pair<const std::string, int>> begin_c, end_c;
        hash_map_const_iterator<std::pair<const std::string, int>> cbegin, cend;
        begin = hm.begin();
        end = hm.end();
        begin_c = hm.begin();
        end_c = hm.end();
        //cbegin = hm.cbegin();
        //cend = hm.cend();

        //(*begin).first = "test"; ERROR
        //(*begin).second = 1; OK

        //(*end).first = "test"; ERROR
        //(*end).second = 1; OK

        //(*begin_c).first = "test"; ERROR
        //(*begin_c).second = 1; ERROR

        //(*end_c).first = "test"; ERROR
        //(*end_c).second = 1; ERROR

        //(*cbegin).first = "test"; ERROR
        //(*cbegin).second = 1; ERROR

        //(*cend).first = "test"; ERROR
        //(*cend).second = 1; ERROR
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
        std::ofstream fout("out.txt");
        using _pair = std::pair<hash_map_iterator<std::pair<const std::string, int>>, bool>;
        hash_map<std::string, int> hm;
        hm.reserve(100);
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
            fout << hm.size() << "/" << hm.max_size() << ":" << hm.load_factor() << "\n";
        }
        hm.max_load_factor(0.1f);
        REQUIRE(hm.max_load_factor() == 0.1f);
        REQUIRE(hm.load_factor() == 0.1f);
        REQUIRE(hm.size() == 100);
        REQUIRE(hm.max_size() == 1000);
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