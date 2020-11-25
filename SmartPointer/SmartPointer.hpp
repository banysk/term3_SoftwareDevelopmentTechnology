#pragma once
#include <memory>
#include "Test.hpp"

namespace smart_pointer {
    // `exception` class definition
class exception :std::exception {
    using base_class = std::exception;
    using base_class::base_class;
};

// `SmartPointer` class declaration
    template<
            typename T,
            typename Allocator
    >
class SmartPointer {
    // don't remove this macro
    ENABLE_CLASS_TESTS;

 public:
    using value_type = T;

    explicit SmartPointer(value_type *ptr = nullptr) {
        ptr == nullptr ? core = nullptr : core = new Core(ptr);
    }

    // copy constructor
    SmartPointer(const SmartPointer &s_ptr) {
        this->~SmartPointer();
        if (s_ptr.core == nullptr) {
            this->core = nullptr;
        } else {
            this->core = s_ptr.core;
            this->core->Inc();
        }
    }

    // move constructor
    SmartPointer(SmartPointer &&s_ptr) {
        this->~SmartPointer();
        if (s_ptr.core == nullptr) {
            this->core = nullptr;
        } else {
            this->core = s_ptr.core;
            s_ptr.core = nullptr;
        }
    }

    // copy assigment
    SmartPointer &operator=(const SmartPointer &s_ptr) {
        this->~SmartPointer();
        if (s_ptr.core == nullptr) {
            this->core = nullptr;
        } else {
            this->core = s_ptr.core;
            this->core->Inc();
        }
        return *this;
    }

    // move assigment
    SmartPointer &operator=(SmartPointer &&s_ptr) {
        this->~SmartPointer();
        if (s_ptr.core == nullptr) {
            this->core = nullptr;
        } else {
            this->core = s_ptr.core;
            s_ptr.core = nullptr;
        }
        return *this;
    }

    //
    SmartPointer &operator=(value_type *ptr) {
        this->~SmartPointer();
        if (ptr == nullptr) {
            this->core = nullptr;
        } else {
            this->core = new Core(ptr);
        }
        return *this;
    }

    ~SmartPointer() {
        if (this->core != nullptr && this->core->getCounter() > 1) {
            this->core->Dec();
        } else {
            delete this->core;
        }
    }

    // return reference to the object of class/type T
    // if SmartPointer contains nullptr throw `SmartPointer::exception`
    value_type &operator*() {
        if (this->core == nullptr) {
            throw smart_pointer::exception();
        } else {
            return *this->core->getPtr();
        }
    }

    const value_type &operator*() const {
        if (this->core == nullptr) {
            throw smart_pointer::exception();
        } else {
            return *this->core->getPtr();
        }
    }

    // return pointer to the object of class/type T
    value_type *operator->() const {
        if (this->core != nullptr) {
            return this->core->getPtr();
        } else {
            return nullptr;
        }
    }

    value_type *get() const {
        if (this->core != nullptr) {
            return this->core->getPtr();
        } else {
            return nullptr;
        }
    }

    // if pointer == nullptr => return false
    operator bool() const {
        if (this->core == nullptr) {
            return false;
        } else {
            return (this->core->getPtr() != nullptr);
        }
    }

    // if pointers points to the same address or both null => true
    template<typename U, typename AnotherAllocator>
    bool operator==(const SmartPointer<U, AnotherAllocator> &s_ptr) const {
        if (this->get() == nullptr && s_ptr.get() == nullptr) {
            return true;
        }
        if (this->get() != nullptr && s_ptr.get() != nullptr) {
            return static_cast<void*>(this->get()) ==
                static_cast<void*>(s_ptr.get());
        }
        return false;
    }

    // if pointers points to the same address or both null => false
    template<typename U, typename AnotherAllocator>
    bool operator!=(const SmartPointer<U, AnotherAllocator> &s_ptr) const {
        if (get() == nullptr && s_ptr.get() == nullptr) {
            return false;
        }
        if (get() != nullptr && s_ptr.get() != nullptr) {
            return !(static_cast<void*>(get()) ==
                static_cast<void*>(s_ptr.get()));
        }
        return true;
    }

    // if smart pointer contains non-nullptr => return count owners
    // if smart pointer contains nullptr => return 0
    std::size_t count_owners() const {
        if (this->core != nullptr) {
            return this->core->getCounter();
        } else {
            return 0;
        }
    }

 private:
    class Core {
     public:
        explicit Core(value_type* ptr) {
            this->_ptr = ptr;
            this->counter = 1;
        }
        ~Core() {
            if (this->_ptr != nullptr) {
                delete this->_ptr;
            }
        }
        void Inc() {
            this->counter++;
        }
        void Dec() {
            this->counter--;
        }
        int getCounter() {
            return this->counter;
        }
        value_type* getPtr() {
            return this->_ptr;
        }
        void setPtr(value_type* ptr) {
            this->_ptr = ptr;
        }

     private:
        value_type* _ptr;
        int counter;
    };
    Core *core;
};
}  // namespace smart_pointer
