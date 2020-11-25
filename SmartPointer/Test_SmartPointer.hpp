#pragma once

#include "SmartPointer.hpp"
#include <cstdint>
#include <memory>
#include <chrono>
#include <random>
#include <list>

using namespace smart_pointer;
using SmartPointer__type_t = SmartPointer<float, std::allocator<float>>;
using SmartPointer__another_type_t = SmartPointer<uint64_t, std::allocator<uint64_t>>;

template<typename value_type>
std::enable_if_t<std::is_floating_point<value_type>::value>
__stress_tests__SmartPointer_operator_star(const std::string& classname) {
    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator*() const") {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<value_type> distribution(0.0, 1.0);

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto raw_ptr = new value_type(distribution(generator));
            const SmartPointer__type_t sptr(raw_ptr);
            ASSERT(*sptr == *raw_ptr);
            if (!(*sptr == *raw_ptr)) {
                break;
            }
        }
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator*()") {
        SmartPointer<value_type, std::allocator<value_type>> sptr(new value_type);

        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_real_distribution<value_type> distribution(0.0, 1.0);

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto value = distribution(generator);
            *sptr = value;
            ASSERT(*sptr == value);
            if (!(*sptr == value)) {
                break;
            }
        }
    }
}

template<typename value_type>
std::enable_if_t<std::is_integral<value_type>::value>
__stress_tests__SmartPointer_operator_star(const std::string& classname) {
    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator*() const") {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<value_type> distribution(std::numeric_limits<value_type>::lowest(), std::numeric_limits<value_type>::max());

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto raw_ptr = new value_type(distribution(generator));
            const SmartPointer__type_t sptr(raw_ptr);
            ASSERT(*sptr == *raw_ptr);
            if (!(*sptr == *raw_ptr)) {
                break;
            }
        }
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator*()") {
        SmartPointer<value_type, std::allocator<value_type>> sptr(new value_type);

        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<value_type> distribution(
                std::numeric_limits<value_type>::lowest(),
                std::numeric_limits<value_type>::max()
        );

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto value = distribution(generator);
            *sptr = value;
            ASSERT(*sptr == value);
            if (!(*sptr == value)) {
                break;
            }
        }
    }
}

template<typename value_type>
std::enable_if_t<!std::is_integral<value_type>::value && !std::is_floating_point<value_type>::value>
__stress_tests__SmartPointer_operator_star(const std::string& classname) {}

DEFINE_CLASS_TESTS(SmartPointer__type_t) {
    TEST_BLOCK("sizeof(SmartPointer)") {
        ASSERT(sizeof(SmartPointer__type_t) == sizeof(SmartPointer__type_t::core));
    }

    TEST_BLOCK("Construct without args") {
        SmartPointer__type_t sptr;
        ASSERT(sptr.core == nullptr);
    }

    TEST_BLOCK("Construct with args") {
        auto raw_ptr = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr(raw_ptr);
        ASSERT(sptr.core != nullptr);
    }

    TEST_BLOCK("Copy-constructor with core[nullptr]") {
        SmartPointer__type_t sptr1, sptr2(sptr1);
        ASSERT(sptr2.core == nullptr);
    }

    TEST_BLOCK("Copy-constructor with core[non-nullptr]") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2(sptr1);
        ASSERT(sptr1.core != nullptr);
        ASSERT(sptr2.core == sptr1.core);
    }

    TEST_BLOCK("Move constructor with core[nullptr]") {
        SmartPointer__type_t sptr1, sptr2(std::move(sptr1));
        ASSERT(sptr1.core == nullptr);
        ASSERT(sptr2.core == nullptr);
    }

    TEST_BLOCK("Move constructor with core[non-nullptr]") {
        auto raw_ptr = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1(raw_ptr), sptr2(std::move(sptr1));
        ASSERT(sptr1.core == nullptr);
        ASSERT(sptr2.core != nullptr);
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator=(const SmartPointer[nullptr]&)") {
        SmartPointer__type_t sptr1, sptr2;
        sptr2 = sptr1;
        ASSERT(sptr1.core == nullptr);
        ASSERT(sptr2.core == nullptr);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator=(const SmartPointer[nullptr]&)") {
        auto raw_ptr = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1, sptr2(raw_ptr);
        sptr2 = sptr1;
        ASSERT(sptr1.core == nullptr);
        ASSERT(sptr2.core == nullptr);
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator=(const SmartPointer[non-nullptr]&)") {
        auto raw_ptr = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1(raw_ptr), sptr2;
        sptr2 = sptr1;
        ASSERT(sptr1.core != nullptr);
        ASSERT(sptr2.core == sptr1.core);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator=(const SmartPointer[non-nullptr]&)") {
        auto raw_ptr1 = new SmartPointer__type_t::value_type;
        auto raw_ptr2 = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1(raw_ptr1), sptr2(raw_ptr2);
        sptr2 = sptr1;
        ASSERT(sptr1.core != nullptr);
        ASSERT(sptr2.core == sptr1.core);
    }

    TEST_BLOCK("[same] SmartPointer[non-nullptr]::operator=(const SmartPointer[non-nullptr]&)") {
        auto raw_ptr1 = new SmartPointer__type_t::value_type;
        auto raw_ptr2 = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1(raw_ptr1), sptr2(raw_ptr2);

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            sptr2 = sptr1;
        }

        ASSERT(sptr1.core != nullptr);
        ASSERT(sptr2.core == sptr1.core);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator=(SmartPointer[non-nullptr]&&)") {
        auto raw_ptr1 = new SmartPointer__type_t::value_type;
        auto raw_ptr2 = new SmartPointer__type_t::value_type;
        SmartPointer__type_t sptr1(raw_ptr1), sptr2(raw_ptr2);
        sptr2 = std::move(sptr1);
        ASSERT(sptr1.core == nullptr);
        ASSERT(sptr2.core != nullptr);
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator=(T*[nullptr])") {
        SmartPointer__type_t sptr;
        sptr = nullptr;
        ASSERT(sptr.core == nullptr);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator=(T*[nullptr])") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);
        sptr = nullptr;
        ASSERT(sptr.core == nullptr);
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator=(T*[non-nullptr])") {
        SmartPointer__type_t sptr;
        auto raw_ptr = new SmartPointer__type_t::value_type;
        sptr = raw_ptr;
        ASSERT(sptr.core != nullptr);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator=(T*[non-nullptr])") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);
        auto raw_ptr = new SmartPointer__type_t::value_type;
        sptr = raw_ptr;
        ASSERT(sptr.core != nullptr);
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator=(T*[non-nullptr])") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);
        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto raw_ptr = new SmartPointer__type_t::value_type;
            sptr = raw_ptr;
            ASSERT(sptr.core != nullptr);
            if (!(sptr.core != nullptr)) {
                break;
            }
        }
    }

    TEST_BLOCK("const SmartPointer[nullptr]::operator*()") {
        const SmartPointer__type_t sptr;
        CHECK_THROW_WITH_TYPE_TRAIT(*sptr, smart_pointer::exception);
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator*()") {
        SmartPointer__type_t sptr;
        CHECK_THROW_WITH_TYPE_TRAIT(*sptr = SmartPointer__type_t::value_type(), smart_pointer::exception);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator*() const") {
        auto raw_ptr = new SmartPointer__type_t::value_type;
        const SmartPointer__type_t sptr(raw_ptr);
        ASSERT(*sptr == *raw_ptr);
    }

    __stress_tests__SmartPointer_operator_star<SmartPointer__type_t::value_type>(classname);

    TEST_BLOCK("SmartPointer[nullptr]::operator->() const") {
        SmartPointer__type_t sptr;
        ASSERT(sptr.operator->() == nullptr);
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator->() const") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto raw_ptr = new SmartPointer__type_t::value_type;
            sptr = raw_ptr;
            ASSERT(sptr.operator->() == raw_ptr);
            if (!(sptr.operator->() == raw_ptr)) {
                break;
            }
        }
    }

    TEST_BLOCK("SmartPointer[nullptr]::get() const") {
        SmartPointer__type_t sptr;
        ASSERT(sptr.get() == nullptr);
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::operator->() const") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);

        std::size_t count_epoch = 100000;
        while (count_epoch--) {
            auto raw_ptr = new SmartPointer__type_t::value_type;
            sptr = raw_ptr;
            ASSERT(sptr.get() == raw_ptr);
            if (!(sptr.get() == raw_ptr)) {
                break;
            }
        }
    }

    TEST_BLOCK("SmartPointer[nullptr]::operator bool() const") {
        SmartPointer__type_t sptr;
        ASSERT(!sptr);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::operator bool() const") {
        SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);
        ASSERT(sptr);
    }
    // operator==
    TEST_BLOCK("[same types] SmartPointer[nullptr]::operator==(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1, sptr2;
        ASSERT(sptr1 == sptr2);
    }

    TEST_BLOCK("[not same types] SmartPointer[nullptr]::operator==(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1;
        SmartPointer__another_type_t sptr2;
        ASSERT(sptr1 == sptr2);
    }

    TEST_BLOCK("[same types] SmartPointer[non-nullptr]::operator==(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2;
        ASSERT(!(sptr1 == sptr2));
    }

    TEST_BLOCK("[not same types] SmartPointer[non-nullptr]::operator==(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type);
        SmartPointer__another_type_t sptr2;
        ASSERT(!(sptr1 == sptr2));
    }

    TEST_BLOCK("[same types] SmartPointer[nullptr]::operator==(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1, sptr2(new SmartPointer__type_t::value_type);
        ASSERT(!(sptr1 == sptr2));
    }

    TEST_BLOCK("[not same types] SmartPointer[nullptr]::operator==(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1;
        SmartPointer__another_type_t sptr2(new SmartPointer__another_type_t::value_type);
        ASSERT(!(sptr1 == sptr2));
    }

    TEST_BLOCK("[same types & ptrs] SmartPointer[non-nullptr]::operator==(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2(sptr1);
        ASSERT(sptr1 == sptr2);
    }

    TEST_BLOCK("[same types & not same ptrs] SmartPointer[non-nullptr]::operator==(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2(new SmartPointer__type_t::value_type);
        ASSERT(!(sptr1 == sptr2));
    }

    TEST_BLOCK("[not same types & not same ptrs] SmartPointer[non-nullptr]::operator==(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type);
        SmartPointer__another_type_t sptr2(new SmartPointer__another_type_t::value_type);
        ASSERT(!(sptr1 == sptr2));
    }

    // operator!=
    TEST_BLOCK("[same types] SmartPointer[nullptr]::operator!=(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1, sptr2;
        ASSERT(!(sptr1 != sptr2));
    }

    TEST_BLOCK("[not same types] SmartPointer[nullptr]::operator!=(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1;
        SmartPointer__another_type_t sptr2;
        ASSERT(!(sptr1 != sptr2));
    }

    TEST_BLOCK("[same types] SmartPointer[non-nullptr]::operator!=(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2;
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("[not same types] SmartPointer[non-nullptr]::operator!=(const SmartPointer[nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type);
        SmartPointer__another_type_t sptr2;
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("[same types] SmartPointer[nullptr]::operator!=(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1, sptr2(new SmartPointer__type_t::value_type);
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("[not same types] SmartPointer[nullptr]::operator!=(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1;
        SmartPointer__another_type_t sptr2(new SmartPointer__another_type_t::value_type);
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("[same types & ptrs] SmartPointer[non-nullptr]::operator!=(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2(sptr1);
        ASSERT(!(sptr1 != sptr2));
    }

    TEST_BLOCK("[same types & not same ptrs] SmartPointer[non-nullptr]::operator!=(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type), sptr2(new SmartPointer__type_t::value_type);
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("[not same types & not same ptrs] SmartPointer[non-nullptr]::operator!=(const SmartPointer[non-nullptr]&) const") {
        SmartPointer__type_t sptr1(new SmartPointer__type_t::value_type);
        SmartPointer__another_type_t sptr2(new SmartPointer__another_type_t::value_type);
        ASSERT(sptr1 != sptr2);
    }

    TEST_BLOCK("SmartPointer[nullptr]::count_owners() const") {
        const SmartPointer__type_t sptr;
        ASSERT(sptr.count_owners() == 0);
    }

    TEST_BLOCK("SmartPointer[non-nullptr]::count_owners() const") {
        const SmartPointer__type_t sptr(new SmartPointer__type_t::value_type);
        ASSERT(sptr.count_owners() == 1);
    }

    TEST_BLOCK("[stress] SmartPointer[non-nullptr]::count_owners() const") {
        std::list<SmartPointer__type_t> lst;
        lst.emplace_back(new SmartPointer__type_t::value_type);

        std::size_t count_epoch = 1000;
        while (count_epoch--) {
            const auto& reference = lst.front();
            ASSERT(reference.count_owners() == lst.size());
            if (!(reference.count_owners() == lst.size())) {
                break;
            }
        }
    }
}