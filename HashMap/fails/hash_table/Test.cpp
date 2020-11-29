#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hash_map.hpp"
#include <fstream>

TEST_CASE("Test") {
    // stream for debug
    std::ofstream fout("out.txt");
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
        REQUIRE(hm.size() == 0);
        REQUIRE(hm.cbegin().operator->() == nullptr);
        REQUIRE(hm.cend().operator->() == nullptr);
    }

    SECTION("hash_map", "hash_map(size_type n)") {
        {
            fefu::hash_map<std::string, int> hm(10);
            REQUIRE(hm.empty());
            REQUIRE(hm.max_size() == 10);
            REQUIRE(hm.size() == 0);
            REQUIRE(hm.cbegin().operator->() != nullptr);
            REQUIRE(hm.cend().operator->() != nullptr);
        }
        {
            fefu::hash_map<std::string, int> hm(0);
            REQUIRE(hm.empty());
            REQUIRE(hm.max_size() == 0);
            REQUIRE(hm.size() == 0);
            REQUIRE(hm.cbegin().operator->() == nullptr);
            REQUIRE(hm.cend().operator->() == nullptr);
        }
    }

    SECTION("hash_map") {
        fefu::hash_map<std::string, int> hm(10);
        std::pair<fefu::hash_map_iterator<std::pair<const std::string, int>>, bool> ans1 = hm.insert({ "test", 1 });
        fout << ans1.first.operator->() << " " << ans1.second << "\n";
        fout << ans1.first->first << " " << ans1.first->second << "\n====================\n";
        
        fefu::hash_map<std::string, int> hm1(hm);
        std::pair<fefu::hash_map_iterator<std::pair<const std::string, int>>, bool> ans2 = hm1.insert({ "test", 10 });
        fout << ans2.first.operator->() << " " << ans2.second << "\n";
        fout << ans2.first->first << " " << ans2.first->second << "\ntest1====================\n";

        REQUIRE(ans1.first->first == ans2.first->first);
        REQUIRE(ans1.second != ans2.second);

        fefu::hash_map<std::string, int> hm2 = move(hm);
        REQUIRE(hm.begin() == nullptr);
        REQUIRE(hm.end() == nullptr);

        std::pair<fefu::hash_map_iterator<std::pair<const std::string, int>>, bool> ans3 = hm2.insert({ "test", 10 });
        REQUIRE(ans1.first->first == ans3.first->first);
        REQUIRE(ans1.second != ans3.second);

        fefu::hash_map<std::string, int> hm3(1);
        hm3.insert({ "1", 1 });
        REQUIRE(hm3.size() == 1);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\n";
        hm3.insert({ "1", 2 });
        REQUIRE(hm3.size() == 2);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\n";
        hm3.insert({ "2", 2 });
        REQUIRE(hm3.size() == 3);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\n";
        hm3.insert({ "2", 3 });
        REQUIRE(hm3.size() == 4);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\n";
        hm3.insert({ "3", 3 });
        REQUIRE(hm3.size() == 5);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\n";
        hm3.insert({ "4", 4 });
        REQUIRE(hm3.size() == 6);
        fout << hm3.size() << "/" << hm3.max_size() << " " << hm3.load_factor() << "\ntest2====================\n";

        fefu::allocator<std::pair<std::string, int>> alloc;
        fefu::hash_map<std::string, int> hm4(alloc);
        hm4.max_load_factor(0.9);
        hm4.reserve(1000);
        for (int i = 1; i <= 1000; i++) {
            hm4.insert({ std::to_string(1), i });
            REQUIRE(hm4.size() == i);
            fout << hm4.size() << "/" << hm4.max_size() << " " << hm4.load_factor() << "\n";
        }
        fout << "\ntest3====================\n";
        // no output
        std::pair<std::string, int> data[5];
        for (int i = 0; i < 5; i++) {
            data[i] = std::make_pair(std::to_string(i), i);
        }
        fefu::hash_map_iterator<std::pair<std::string, int>> it1_(&data[0]), it2_(&data[4]);
        fefu::hash_map<std::string, int> hm5(it1_, it2_);
        REQUIRE(hm5.size() == 4);
        REQUIRE(hm5.max_size() == ceil(4.0 / hm5.max_load_factor()));
        fefu::hash_map<std::string, int> hm6(hm5, alloc);
        REQUIRE(hm6.size() == 4);
        REQUIRE(hm6.max_size() == ceil(4.0 / hm6.max_load_factor()));
        fefu::hash_map<std::string, int> hm7 = move(hm6);
        REQUIRE(hm6.size() == 0);
        REQUIRE(hm6.max_size() == 0);
        REQUIRE(hm7.size() == 4);
        REQUIRE(hm7.max_size() == ceil(4.0 / hm7.max_load_factor()));
        // no output
        std::pair<std::string, int> a("1", 1), b("2", 2), c("3",3);
        fefu::hash_map<std::string, int> hm8({a, b, c});
        REQUIRE(hm8.size() == 3);
        REQUIRE(hm8.max_size() == ceil(3.0 / hm8.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm9 = hm8;
        REQUIRE(hm8.size() == 3);
        REQUIRE(hm8.max_size() == ceil(3.0 / hm8.max_load_factor()));
        REQUIRE(hm9.size() == 3);
        REQUIRE(hm9.max_size() == ceil(3.0 / hm9.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm10 = move(fefu::hash_map<std::string, int>({ a, b, c }));
        REQUIRE(hm10.size() == 3);
        REQUIRE(hm10.max_size() == ceil(3.0 / hm9.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm11 = {a, b, c};
        REQUIRE(hm11.size() == 3);
        REQUIRE(hm11.max_size() == ceil(3.0 / hm11.max_load_factor()));
        // no output 
        hm4 = { a, b, c };
        REQUIRE(hm4.size() == 3);
        REQUIRE(hm4.max_size() == ceil(3.0 / hm4.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm12;
        hm12.insert(it1_, it2_);
        REQUIRE(hm12.size() == 4);
        REQUIRE(hm12.max_size() == ceil(4.0 / hm12.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm13;
        hm13.insert({a, b, c});
        REQUIRE(hm13.size() == 3);
        REQUIRE(hm13.max_size() == ceil(3.0 / hm13.max_load_factor()));
        // no output
        fefu::hash_map<std::string, int> hm14;
        std::pair<fefu::hash_map_iterator<std::pair<const std::string, int>>, bool> ans4 = hm14.insert({ "test", 1 });
        REQUIRE(hm14.size() == 1);
        REQUIRE(hm14.max_size() == ceil(hm14.size() / hm14.max_load_factor()));
        std::pair<fefu::hash_map_iterator<std::pair<const std::string, int>>, bool> ans5 = hm14.insert({ "test1", 100 });
        REQUIRE(hm14.size() == 2);
        REQUIRE(hm14.max_size() == ceil(hm14.size() / hm14.max_load_factor()));
        hm14.erase(ans4.first);
        REQUIRE(hm14.size() == 1);
        hm14.erase(ans5.first);
        REQUIRE(hm14.size() == 0);
        // no output 
        //hm4.clear();
        fefu::hash_map<std::string, int> hm15;
        //hm15.clear();
    }
    
}