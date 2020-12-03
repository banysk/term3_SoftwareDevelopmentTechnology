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
        REQUIRE(hm.max_size() == 20);
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
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        allocator<std::pair<const std::string, int >> alloc;
        hash_map<std::string, int> hm(hash_map<std::string, int>({ a, b, c, d, e }), alloc);
        REQUIRE(hm.size() == 5);
    }
       
    SECTION("hash_map", "[hash_map(std::initializer_list<value_type> l, size_type n = 0)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm({ a, b, c, d, e }, 5);
        REQUIRE(hm.size() == 5);
    }

    SECTION("hash_map", "[hash_map& operator=(const hash_map&)") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hash_map<std::string, int> hm1({ a, b, c, d, e }, 5), hm2;
        hm2 = hm1;
        REQUIRE(hm1.size() == 5);
        REQUIRE(hm2.size() == 5);
    }

    SECTION("hash_map", "[hash_map& operator=(hash_map&&)") {
        hash_map<std::string, int> hm;
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        hm = hash_map<std::string, int>({ a, b, c, d, e }, 5);
        REQUIRE(hm.size() == 5);
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
        std::ofstream fout("out1.txt");
        hash_map<std::string, int> hm;
        auto it_in = hm.insert({ "1", 1 });
        auto it_er = hm.erase(it_in.first);
        REQUIRE(it_er == hm.end());
        hm.reserve(2);
        auto it1_in = hm.insert({ "0", 0 });
        auto it2_in = hm.insert({ "5", 0 });
        fout << "insert\n";
        fout << (it1_in.first).operator->() << "\n";
        fout << (it2_in.first).operator->() << "\n";
        fout << (it1_in.first).operator->() - (it2_in.first).operator->() << "\n";
        fout << "erase\n";
        auto er = hm.erase(it2_in.first);
        fout << er.operator->();
        REQUIRE(it1_in.first == er);
        hash_map_const_iterator<std::pair<const std::string, int>> it(it1_in.first);
        auto const_er = hm.erase(it);
        REQUIRE(const_er == hm.end());
    }

    SECTION("hash_map", "[size_type erase(const key_type& x)]") {

    }

    SECTION("hash_map", "[iterator erase(const_iterator first, const_iterator last)]") {

    }

    SECTION("hash_map", "[void swap(hash_map& x)]") {

    }

    SECTION("hash_map", "[void merge(hash_map<K, T, _H2, _P2, Alloc>& source)]") {
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        std::pair<const std::string, int> f("6", 6), g("7", 7), h("8", 8), i("9", 9), j("10", 10);
        hash_map<std::string, int> hm1({a, b, c, d, e});
        hm1.merge(hash_map<std::string, int>({f, g, h, i, j}));
        REQUIRE(hm1.size() == 10);
        hash_map<std::string, int> hm2({ a, b, c, d, e }), hm3({ f, g, h, i, j });
        hm2.merge(hm3);
        REQUIRE(hm2.size() == 10);
    }

    SECTION("hash_map", "[void merge(hash_map<K, T, _H2, _P2, Alloc>&& source)]") {

    }

    SECTION("hash_map", "[Hash hash_function() const]") {

    }

    SECTION("hash_map", "[Pred key_eq() const]") {

    }

    SECTION("hash_map", "[iterator find(const key_type& x)]") {
        std::ofstream fout("out2.txt");
        hash_map<std::string, int> hm(3);
        auto in1 = hm.insert({"0", 0}); // +1 -> 1
        auto in2 = hm.insert({"1", 1}); // +1 -> 2
        auto in3 = hm.insert({"2", 2}); // +1 -> 3
        hm.erase(in2.first); // -1 -> 2
        std::string key1 = "0", key2 = "1", key3 = "2";
        REQUIRE(in1.first == hm.find(key1));
        REQUIRE(hm.end() == hm.find(key2));
        REQUIRE(in3.first == hm.find(key3));
        hash_map_const_iterator<std::pair<const std::string, int>> cin1(in1.first), cin3(in3.first);
        hash_map_const_iterator<std::pair<const std::string, int>> c1, c3;
        c1 = hm.find(key1);
        c3 = hm.find(key3);
        REQUIRE(cin1 == c1);
        REQUIRE(cin3 == c3);
        REQUIRE(hm.count(key1) == 1);
        REQUIRE(hm.count(key2) == 0);
        REQUIRE(hm.count(key3) == 1);
        REQUIRE(hm.contains(key1) == true);
        REQUIRE(hm.contains(key2) == false);
        REQUIRE(hm.contains(key3) == true);
        REQUIRE(hm.bucket(key1) != -1);
        REQUIRE(hm.bucket(key2) == -1);
        REQUIRE(hm.bucket(key3) != -1);
        REQUIRE(hm["0"] == 0);
        REQUIRE(hm["1"] == 0); // +1 -> 3
        REQUIRE(hm["2"] == 2);
        std::string &&lvalue = "0";
        REQUIRE(hm[lvalue] == 0);
        hm.erase("0"); // -1 -> 2
        REQUIRE(hm.contains(key1) == false);
        hm.insert({"3", 3}); // +1 -> 3
        REQUIRE(hm.size() == 3);
        hm.erase(hm.begin(), hm.end()); // -3 -> 0
        REQUIRE(hm.size() == 0);
        hm["777"] = 100; // +1 -> 1 
        hm["111"] = 111; // +1 -> 2
        REQUIRE(hm.find("777") != hm.end());
        REQUIRE(hm.at("777") == hm["777"]);
        REQUIRE_THROWS(hm.at("-1"));
        const int a = hm.at("777");
        REQUIRE(hm.size() == 2);
        hash_map<std::string, int> hm1, hm2;
        hm1.insert({ "0", 0 });
        //hm.swap(hm1);
        //int i = 0;
        //for (auto el : hm1) {
        //    i++;
        //}
        //REQUIRE(i == 2);
        hm1 == hm2;
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

        std::ofstream fout("out3.txt");
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