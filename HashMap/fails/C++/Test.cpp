#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hash_map.hpp"
#include <fstream>

TEST_CASE("Test") {

	SECTION("allocator")
    {
        fefu::allocator<int> alloc;
        int* mem = alloc.allocate(100);
        alloc.deallocate(mem, 100);
	}

    SECTION("hash_map_iterator") {
        fefu::allocator<int> alloc;
        fefu::hash_map_iterator<int> it_;
        int* ptr_ = alloc.allocate(5);
        it_ = fefu::hash_map_iterator<int>(ptr_);
        REQUIRE(ptr_ == it_.operator->()); // operator->()
        REQUIRE(*ptr_ == *it_.operator->()); // *operator->()
        REQUIRE(*ptr_ == *it_); // operator*()
        REQUIRE(*ptr_++ == *it_++); // prefix++
        ptr_++;
        REQUIRE(*ptr_ == *++it_); // postfix++
        int* new_ptr_ = alloc.allocate(1);
        fefu::hash_map_iterator<int> it1_(new_ptr_), it2_(new_ptr_);
        REQUIRE(it1_ == it2_); // operator==
        it1_++;
        REQUIRE(it1_ != it2_); // operator!=
    }

    SECTION("const_hash_map_iterator") {
        fefu::allocator<int> alloc;
        fefu::hash_map_const_iterator<int> it_;
        int* ptr_ = alloc.allocate(5);
        it_ = fefu::hash_map_const_iterator<int>(ptr_);
        REQUIRE(ptr_ == it_.operator->()); // operator->()
        REQUIRE(*ptr_ == *it_.operator->()); // *operator->()
        REQUIRE(*ptr_ == *it_); // operator*()
        REQUIRE(*ptr_++ == *it_++); // prefix++
        ptr_++;
        REQUIRE(*ptr_ == *++it_); // postfix++
        int* new_ptr_ = alloc.allocate(1);
        fefu::hash_map_const_iterator<int> it1_(new_ptr_), it2_(new_ptr_);
        REQUIRE(it1_ == it2_); // operator==
        it1_++;
        REQUIRE(it1_ != it2_); // operator!=
    }

    SECTION("hash_map", "hash_map()") {
        fefu::hash_map<std::string, int> hm;
        REQUIRE(hm.empty());
        REQUIRE(hm.max_size() == 0);
        REQUIRE(hm.bucket_count() == 0);
        REQUIRE(hm.cbegin().operator->() == nullptr);
        REQUIRE(hm.cend().operator->() == nullptr);
    }

    SECTION("hash_map", "hash_map(size_type n)") {
        {
            fefu::hash_map<std::string, int> hm(10);
            REQUIRE(hm.empty());
            REQUIRE(hm.max_size() == 10);
            REQUIRE(hm.bucket_count() == 0);
            REQUIRE(hm.cbegin().operator->() != nullptr);
            REQUIRE(hm.cend().operator->() != nullptr);
        }
        {
            fefu::hash_map<std::string, int> hm(0);
            REQUIRE(hm.empty());
            REQUIRE(hm.max_size() == 0);
            REQUIRE(hm.bucket_count() == 0);
            REQUIRE(hm.cbegin().operator->() == nullptr);
            REQUIRE(hm.cend().operator->() == nullptr);
        }
    }

    SECTION("hash_map", "hash_map(size_type n)") {
        std::ofstream fout("output.txt");
        fefu::hash_map<std::string, int> hm(10);
        std::pair<std::string, int> a = std::make_pair<std::string, int>("test", 1);
        REQUIRE(a.first == "test");
        REQUIRE(a.second == 1);
        hm.insert(a);
        std::pair<std::string, int>* mass = new std::pair<std::string, int>[10];
        *mass = std::make_pair("1", 1);
    }
}