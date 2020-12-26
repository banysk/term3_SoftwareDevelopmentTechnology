#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hash_map.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<std::string> log_data;

TEST_CASE("allocator") {
    fefu::allocator<int> alloc;
    int *data = alloc.allocate(100);
    alloc.deallocate(data, 100);
}

TEST_CASE("hash_map") {
   
    SECTION("1") {
        log_data.push_back("SECTION(\"1\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm;
        hm.insert({ "1", 5 });
        REQUIRE(hm.size() == 1);
        hm.insert({ "2", 4 });
        REQUIRE(hm.size() == 2);
        hm.insert({ "3", 3 });
        REQUIRE(hm.size() == 3);
        hm.insert({ "4", 2 });
        REQUIRE(hm.size() == 4);
        hm.insert({ "5", 1 });
        REQUIRE(hm.size() == 5);
        hm.insert({ "5", 0 });
        REQUIRE(hm.size() == 5);
    }

    SECTION("2") {
        log_data.push_back("SECTION(\"2\")\n");
        log_data.push_back("<no data>\n");
        std::pair<const std::string, int> a[] = { 
            {"1", 5}, {"2", 4}, {"3", 3}, {"4", 2}, {"5", 1} 
        };
        fefu::hash_map<std::string, int> hm;
        for (int i = 1; i < 6; i++) {
            hm.insert(a[i - 1]);
            REQUIRE(hm.size() == i);
        }
    }

    SECTION("3") {
        log_data.push_back("SECTION(\"3\")\n");
        std::pair<const std::string, int> a[] = {
            {"1", 5}, {"2", 4}, {"3", 3}, {"4", 2}, {"5", 1}
        };
        fefu::hash_map<std::string, int> hm;
        for (int i = 1; i < 6; i++) {
            hm.insert(a[i - 1]);
        }
        for (auto el : hm) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + "\n");
        }
    }

    SECTION("4") {
        log_data.push_back("SECTION(\"4\")\n");
        std::pair<const std::string, int> a[] = {
            {"str1", 5}, {"str2", 4}, {"str3", 3}, {"str4", 2}, {"str5", 1}
        };
        fefu::hash_map<std::string, int> hm;
        for (int i = 1; i < 6; i++) {
            hm.insert(a[i - 1]);
        }
        fefu::hash_map<std::string, int> hm1(hm.begin(), hm.end(), 5);
        log_data.push_back("___hm___\n");
        for (auto el : hm) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + "\n");
        }
        log_data.push_back("___hm1___\n");
        for (auto el : hm1) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + "\n");
        }
        REQUIRE(hm.size() == hm1.size());
    }

    SECTION("5") {
        log_data.push_back("SECTION(\"5\")\n");
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        fefu::hash_map<std::string, int> hm({a, b, c, d, e}, 5);
        log_data.push_back("___hm___\n");
        for (auto el : hm) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + "\n");
        }
        REQUIRE(hm.size() == 5);
    }

    SECTION("6") {
        log_data.push_back("SECTION(\"6\")\n");
        log_data.push_back("<no data>\n");
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        fefu::hash_map<std::string, int> hm({ a, b, c, d, e }, 5);
        fefu::hash_map<std::string, int> hm1;
        hm1.insert(hm.begin(), hm.end());
        REQUIRE(hm1.size() == 5);
    }

    SECTION("7") {
        log_data.push_back("SECTION(\"7\")\n");
        log_data.push_back("<no data>\n");
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        fefu::hash_map<std::string, int> hm;
        hm.insert({ a, b, c, d, e });
        REQUIRE(hm.size() == 5);
    }

    SECTION("8") {
        log_data.push_back("SECTION(\"8\")\n");
        log_data.push_back("<no data>\n");
        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        fefu::hash_map<std::string, int> hm;
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
        }
        REQUIRE(hm.size() == 100);
        hm = { a, b, c, d, e };
        REQUIRE(hm.size() == 5);
    }

    SECTION("9") {
        log_data.push_back("SECTION(\"9\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm(1000);
        for (int i = 0; i < 1000; i++) {
            auto it = hm.insert({ std::to_string(i), i });
            REQUIRE(hm.find(std::to_string(i)) == it.first);
            if (i > 0) {
                hm.erase(std::to_string(i - 1));
            }
        }
    }

    SECTION("10") {
        log_data.push_back("SECTION(\"10\")\n");
        fefu::hash_map<std::string, int> hm(1000);
        for (int i = 0; i < 20; i++) {
            hm.insert({ std::to_string(i), i });
        }
        for (auto el : hm) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + " => ");
            log_data.push_back(std::to_string(hm.bucket(el.first)) + "\n");
        }
        fefu::hash_map<std::string, int> hm1;
        hm1.swap(hm);
        REQUIRE(hm1.size() == 20);
        REQUIRE(hm.size() == 0);
        log_data.push_back("++++++++++\n");
        for (auto el : hm1) {
            log_data.push_back("\"" + el.first + "\":=" + std::to_string(el.second) + " => ");
            log_data.push_back(std::to_string(hm1.bucket(el.first)) + "\n");
        }
    }

    SECTION("11") {
        log_data.push_back("SECTION(\"11\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm1, hm2;
        hm1.max_load_factor(0.5);
        hm2.max_load_factor(0.25);
        for (int i = 0; i < 100; i++) {
            hm1.insert({ std::to_string(i), i });
            REQUIRE(hm1.size() == i + 1);
            hm2.insert({ std::to_string(99 - i), 99 - i });
            REQUIRE(hm2.size() == i + 1);
        }
        std::vector<int> v1, v2;
        for (auto el : hm1) {
            v1.push_back(el.second);
        }
        for (auto el : hm2) {
            v2.push_back(el.second);
        }
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
        log_data.push_back(std::to_string(v1.size()) + " | " + std::to_string(v2.size()) + '\n');
        REQUIRE(v1 == v2);
        REQUIRE(hm1 == hm2);
    }

    SECTION("12") {
        log_data.push_back("SECTION(\"12\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm;
        REQUIRE_THROWS(hm.at("1"));
        REQUIRE(hm["a"] == 0);
        hm["zzz"] = 1111;
        REQUIRE(hm.at("zzz") == 1111);
        const std::string key = "123";
        hm[key] = 123;
        REQUIRE(hm.count(key) == 1);
        REQUIRE(hm.contains(key) == true);
        REQUIRE(hm.at(key) == 123);
        hm["1"];
        hm.erase(key);
        hm[key];
    }

    SECTION("13") {
        log_data.push_back("SECTION(\"13\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm;
        for (int i = 0; i < 100; i++) {
            hm.insert({ std::to_string(i), i });
        }
        fefu::hash_map<std::string, int> hm1(hm);
        REQUIRE(hm == hm1);

        std::pair<const std::string, int> a("1", 1), b("2", 2), c("3", 3), d("4", 4), e("5", 5);
        fefu::hash_map<std::string, int> hm2 = move(fefu::hash_map<std::string, int>({a, b, c, d, e}));
        REQUIRE(hm2.size() == 5);
    }

    SECTION("14") {
        log_data.push_back("SECTION(\"14\")\n");
        log_data.push_back("<no data>\n");
        fefu::allocator<std::pair<const std::string, int>> a;
        fefu::hash_map<std::string, int> hm1(a);
        for (int i = 0; i < 100; i++) {
            hm1.insert({ std::to_string(i), i });
        }
        fefu::hash_map<std::string, int> hm2;
        for (int i = 100; i < 200; i++) {
            hm1.insert({ std::to_string(i), i });
        }
        hm1.merge(hm2);
        REQUIRE(hm1.size() == 200);

        fefu::hash_map<std::string, int> hm3(hm1, a);
        REQUIRE(hm3.size() == 200);

        fefu::hash_map<std::string, int> hm4(fefu::hash_map<std::string, int>(), a);
        REQUIRE(hm4.size() == 0);
    }
    
    SECTION("15") {
        log_data.push_back("SECTION(\"15\")\n");
        log_data.push_back("<no data>\n");
        fefu::hash_map<std::string, int> hm1(100);
        for (int i = 0; i < 100; i++) {
            hm1.insert({ std::to_string(i), i });
        }
        fefu::hash_map<std::string, int> hm2;
        hm2 = hm1;
        REQUIRE(hm1 == hm2);

        hm2 = fefu::hash_map<std::string, int>({ {"1", 1}, {"2", 2} });
        REQUIRE(hm2.size() == 2);
    }

    SECTION("16") {
        log_data.push_back("SECTION(\"15\")\n");
        fefu::hash_map<std::string, int> hm;
        hm.emplace(std::piecewise_construct,
            std::forward_as_tuple("test"),
            std::forward_as_tuple(777));
        log_data.push_back(std::to_string(hm["test"]) + "\n");
        hm.try_emplace("test", 666);
        log_data.push_back(std::to_string(hm["test"]) + "\n");
        std::string key = "test1";
        hm.emplace(key, 1);
        log_data.push_back(std::to_string(hm[key]) + "\n");
        hm.try_emplace(key, 2);
        log_data.push_back(std::to_string(hm[key]) + "\n");
        hm.insert_or_assign("testTTT", 10101010);
        log_data.push_back(std::to_string(hm["testTTT"]) + "\n");
        hm.insert_or_assign(key, -2);
        log_data.push_back(std::to_string(hm[key]) + "\n");
    }

    SECTION("LOG") {
        std::ofstream fout("log.txt");
        for (auto line : log_data) {
            fout << line;
        }
    }
}
