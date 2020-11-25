#pragma once
#include <map>
#include <string>
#include <vector>
#include <functional>
// be faster!!!
class GameDatabase {
 public:
     GameDatabase() = default;

     void Insert(ObjectId id, string name, size_t x, size_t y) {
         data[id] = { id, name, x, y };
     }

     void Remove(ObjectId id) {
         data.erase(id);
     }

     vector<GameObject> DataByName(string name) const {
         std::vector<GameObject> out;
         for (auto elem : data) {
             if (elem.second.name == name) {
                 out.push_back(elem.second);
             }
         }
         return out;
     }

     vector<GameObject> DataByPosition(size_t x, size_t y) const {
         std::vector<GameObject> out;
         for (auto elem : data) {
             if (elem.second.x == x && elem.second.y == y) {
                 out.push_back(elem.second);
             }
         }
         return out;
     }

     vector<GameObject> Data() const {
         std::vector<GameObject> out;
         for (auto elem : data) {
             out.push_back(elem.second);
         }
         return out;
     }

 private:
     std::map<ObjectId, GameObject, std::greater<ObjectId>> data;
};
