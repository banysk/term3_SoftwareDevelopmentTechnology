#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <memory>
#include <map>
#include <iomanip>
#include <sstream>
#include <cassert>
#include "factory.h"

using std::string;
using std::cout;
using std::cin;
using std::vector;
using std::endl;

string class_id;

class Smth : public Object {
public:
    Smth(const std::string& class_id) {
        class_id_ = class_id;
    }

    std::string ToString() const override {
        return class_id_;
    }
private:
    std::string class_id_;
};

Object* my_new_smth_() {
    return new Smth(class_id);
}

int main() {
    getline(cin, class_id);
    Factory factory;
    if (class_id != "apple!" && class_id != "list" && class_id != "yet another identifier") {
        factory.Register(class_id, my_new_smth_);
    }
    Object* smth_instance_ptr = factory.Create(class_id);
    cout << smth_instance_ptr->ToString() << endl;
    delete smth_instance_ptr;
}
