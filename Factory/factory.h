#pragma once
#include <string>
#include <map>

class Object {
 public:
     virtual std::string ToString() const = 0;
     virtual ~Object() {}
};

class Child : public Object {
 public:
     explicit Child(const std::string& class_id) {
         class_id_ = class_id;
     }

     std::string ToString() const override {
         return class_id_;
     }
 private:
     std::string class_id_;
};

class Factory {
 public:
     Factory() {
         std::string in[] = {
             "apple!",
             "list",
             "yet another identifier"
         };
         for (auto id : in) {
             data[id] = [=]() -> Object* {return new Child(id); };
         }
     }
     Object* Create(const std::string& class_id) {
         return data.find(class_id)->second();
     }
     void Register(const std::string& class_id, Object*(*instance_creator)()) {
         data[class_id] = instance_creator;
     }

 private:
     std::map<std::string, std::function<Object*()>> data;
};
