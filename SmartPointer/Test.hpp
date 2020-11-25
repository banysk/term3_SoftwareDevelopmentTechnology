#pragma once

#ifdef ENABLE_TESTS

#include <iostream>
#include <string>

#if defined(__clang__)
#define DEMANGLE(CLASS) CLASS
#elif defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#include <memory>
namespace test {
    static std::string demangle(const char* name) {
        auto demangledName = abi::__cxa_demangle(name, 0, 0, nullptr);
        auto result = std::string(demangledName);
        free(demangledName);
        return result;
    }
}
#define DEMANGLE(CLASS) test::demangle(CLASS)
#elif defined(_MSC_VER)
#define DEMANGLE(CLASS) CLASS
#endif

namespace test {
    template<class __TEST_CLASSNAME__>
    static void test(const std::string& classname) {
        std::cout << "[Test][Error]: `" << classname << "`'s tests undefined" << std::endl;
    }
}

#define DEFINE_CLASS_TESTS(__TEST_CLASSNAME__) template<> void test::test<__TEST_CLASSNAME__>(const std::string& classname)
#define ENABLE_CLASS_TESTS template<class __TEST_TEMPLATE_PARAMETERS__> friend void test::test(const std::string& classname)
#define EXECUTE_CLASS_TEST(__TEST_CLASSNAME__) test::test<__TEST_CLASSNAME__>(DEMANGLE(typeid(__TEST_CLASSNAME__).name()))
#define EXECUTE_TESTS_FOR_EACH(...) test::execute_all_tests<__VA_ARGS__>()
#define TEST_BLOCK(NAME) for (std::string TEST_BLOCK_NAME = #NAME; !TEST_BLOCK_NAME.empty(); TEST_BLOCK_NAME.clear()) for (std::size_t TEST_BLOCK_COUNTER = 0; TEST_BLOCK_COUNTER != 1; TEST_BLOCK_COUNTER = 1)
#define ASSERT(EXPR) { \
    if (!static_cast<bool>(EXPR)) { \
        std::cout << "[" << TEST_BLOCK_NAME << "]" << "[" << ++TEST_BLOCK_COUNTER << "]"; \
        std::cout << __FILE__ << ":" << __LINE__ << ": Assertion `" << #EXPR << "` failed" << std::endl; \
    } \
}
#define CHECK_THROW(EXPR) { \
    auto __TEST_EXCEPTION_THROWN__ = false; \
    try { \
        EXPR; \
    } catch(...) { \
        __TEST_EXCEPTION_THROWN__ = true; \
    } \
    if (!__TEST_EXCEPTION_THROWN__) { \
        std::cout << "[" << TEST_BLOCK_NAME << "]" << "[" << ++TEST_BLOCK_COUNTER << "]"; \
        std::cout << __FILE__ << ":" << __LINE__ << ": no throw from `" << #EXPR << "` failed" << std::endl; \
    } \
}
#define CHECK_THROW_WITH_TYPE_TRAIT(EXPR, EXCEPTION_TYPE) { \
    auto __TEST_EXCEPTION_THROWN__ = false; \
    auto __TEST_EXCEPTION_TRAIT_MATCH__ = false; \
    try { \
        EXPR; \
    } catch(EXCEPTION_TYPE& ex) { \
        __TEST_EXCEPTION_THROWN__ = true; \
        __TEST_EXCEPTION_TRAIT_MATCH__ = true; \
    } catch(...) { \
        __TEST_EXCEPTION_THROWN__ = true; \
    } \
    if (!__TEST_EXCEPTION_THROWN__) { \
        std::cout << "[" << TEST_BLOCK_NAME << "]" << "[" << ++TEST_BLOCK_COUNTER << "]"; \
        std::cout << __FILE__ << ":" << __LINE__ << ": no exceptions were thrown from `" << #EXPR << "` failed" << std::endl; \
    } \
    if (!__TEST_EXCEPTION_TRAIT_MATCH__) { \
        std::cout << "[" << TEST_BLOCK_NAME << "]" << "[" << ++TEST_BLOCK_COUNTER << "]"; \
        std::cout << __FILE__ << ":" << __LINE__ << ": another exception was thrown from `" << #EXPR << "` failed" << std::endl; \
    } \
}

namespace test {
    template <class... Classes>
    void execute_all_tests() {
        std::initializer_list<int>{(EXECUTE_CLASS_TEST(Classes), 0)...};
    }
}

#else

#define DEFINE_CLASS_TESTS(CLASSNAME) template<> void test::test<CLASSNAME>()
#define ENABLE_CLASS_TESTS template<class __TEST_TEMPLATE_PARAMETERS__> friend void test::test()
#define EXECUTE_CLASS_TEST(CLASSNAME)
#define EXECUTE_TESTS_FOR_EACH(...)
#define TEST_BLOCK(NAME) if (false)
#define ASSERT(EXPR) false
#define CHECK_THROW(EXPR)
#define CHECK_THROW_WITH_TYPE_TRAIT(EXPR, EXCEPTION_TYPE)

namespace test {
    template<class __TEST_CLASSNAME__>
    static void test() {}
}

#endif