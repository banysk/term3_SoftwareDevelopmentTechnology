#pragma once
#include <algorithm>
#include <vector>

class Set {
 public:
     Set() {}
     explicit Set(std::vector<int64_t>& vec) : elements(vec) {
     std::sort(elements.begin(), elements.end());
     auto it = std::unique(elements.begin(), elements.end());
     elements.resize(distance(elements.begin(), it));
     }
     std::vector<int64_t> Data() {
         return elements;
     }
     Set Union(const Set& set) const {
         std::vector<int64_t> all_elements;
         for (unsigned int i = 0; i < elements.size(); i++) {
             all_elements.push_back(elements[i]);
         }
         for (unsigned int i = 0; i < set.elements.size(); i++) {
             all_elements.push_back(set.elements[i]);
         }
         std::sort(all_elements.begin(), all_elements.end());
         auto it = std::unique(all_elements.begin(), all_elements.end());
         all_elements.resize(distance(all_elements.begin(), it));
         return Set(all_elements);
     }
     Set Intersection(const Set& set) const {
         std::vector<int64_t> all_elements;
         for (unsigned int i = 0; i < elements.size(); i++) {
             if (set.Contains(elements[i])) {
                 all_elements.push_back(elements[i]);
             }
         }
         std::sort(all_elements.begin(), all_elements.end());
         return Set(all_elements);
     }
     Set Difference(const Set& set) const {
         std::vector<int64_t> all_elements;
         for (unsigned int i = 0; i < elements.size(); i++) {
             if (!set.Contains(elements[i])) {
                 all_elements.push_back(elements[i]);
             }
         }
         std::sort(all_elements.begin(), all_elements.end());
         return Set(all_elements);
     }
     Set SymmetricDifference(const Set& set) const {
         std::vector<int64_t> all_elements;
         for (unsigned int i = 0; i < elements.size(); i++) {
             if (!set.Contains(elements[i])) {
                 all_elements.push_back(elements[i]);
             }
         }
         for (unsigned int i = 0; i < set.elements.size(); i++) {
             if (!Contains(set.elements[i])) {
                 all_elements.push_back(set.elements[i]);
             }
         }
         std::sort(all_elements.begin(), all_elements.end());
         auto it = std::unique(all_elements.begin(), all_elements.end());
         all_elements.resize(distance(all_elements.begin(), it));
         return Set(all_elements);
     }
     bool Contains(int64_t element) const {
         for (unsigned int i = 0; i < elements.size(); i++) {
             if (elements[i] == element) {
                 return true;
            }
        }
         return false;
     }
     void Add(int64_t element) {
         if (!Contains(element)) {
             elements.push_back(element);
             sort(elements.begin(), elements.end());
        }
     }
     void Remove(int64_t element) {
         int64_t place = -1;
         for (unsigned int i = 0; i < elements.size(); i++) {
             if (elements[i] == element) {
                 place = i;
                 break;
             }
         }
         if (place != -1) {
             elements.erase(elements.begin() + place);
         }
     }

 private:
     std::vector<int64_t> elements;
};
