#pragma once
#include <vector>

template<typename Tp>
class FixedAllocator {
 public:
     explicit FixedAllocator(std::uint64_t page_size) :
         page_allocator_(page_size * sizeof(Tp)),
         size(page_size) {}

     Tp* Allocate() {
         if (!memory.size()) {
             Tp* new_memory_begin =
                 static_cast<Tp*>(page_allocator_.Allocate());
             memory.push_back(new_memory_begin);
             for (std::uint64_t _it = 0; _it < size - 1; _it++) {
                 memory.push_back(new_memory_begin++);
             }
         }
         Tp* memory_next = memory[memory.size() - 1];
         memory.resize(memory.size() - 1);
         return memory_next;
     }

     void Deallocate(Tp* p) {
         memory.push_back(p);
     }

     const PageAllocator& InnerAllocator() const noexcept {
         return page_allocator_;
     }

 private:
     PageAllocator page_allocator_;
     std::vector<Tp*> memory;
     std::uint64_t size;
};
