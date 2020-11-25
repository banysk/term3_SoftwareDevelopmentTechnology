#include <iostream>
#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <fstream>

class ClassDeclarationNode;
class VarDeclarationNode;
class MethodDeclarationNode;
class BaseNode;

class BaseVisitor {
  public:
    virtual void Visit(const BaseNode* node) = 0;
    virtual void Visit(const ClassDeclarationNode* node) = 0;
    virtual void Visit(const VarDeclarationNode* node) = 0;
    virtual void Visit(const MethodDeclarationNode* node) = 0;
};

class BaseNode {
  public:
    virtual void Visit(BaseVisitor* visitor) const = 0;
};

class ClassDeclarationNode: public BaseNode {
  public:
    ClassDeclarationNode(std::string class_name,
        std::vector<BaseNode*> public_fields = std::vector<BaseNode*>(),
        std::vector<BaseNode*> protected_fields = std::vector<BaseNode*>(),
        std::vector<BaseNode*> private_fields = std::vector<BaseNode*>())
            : class_name_(class_name)
            , public_fields_(public_fields)
            , protected_fields_(protected_fields)
            , private_fields_(private_fields)
    {}


    const std::string& ClassName() const {
        return class_name_;
    }

    const std::vector<BaseNode*>& PublicFields() const {
        return public_fields_;
    }

    const std::vector<BaseNode*>& ProtectedFields() const {
        return protected_fields_;
    }

    const std::vector<BaseNode*>& PrivateFields() const {
        return private_fields_;
    }

    void Visit(BaseVisitor* visitor) const override {
        visitor->Visit(this);
    }

  private:
    std::string class_name_;
    std::vector<BaseNode*> public_fields_;
    std::vector<BaseNode*> protected_fields_;
    std::vector<BaseNode*> private_fields_;
};

class VarDeclarationNode: public BaseNode {
  public:
    VarDeclarationNode(std::string var_name, std::string type_name)
            : var_name_(var_name), type_name_(type_name) {}

    const std::string& VarName() const {
        return var_name_;
    }

    const std::string& TypeName() const {
        return type_name_;
    }

    void Visit(BaseVisitor* visitor) const override {
        visitor->Visit(this);
    }

  private:
    std::string var_name_;
    std::string type_name_;
};

class MethodDeclarationNode: public BaseNode {
  public:
    MethodDeclarationNode(std::string return_type_name,
        std::string method_name, std::vector<BaseNode*> args = std::vector<BaseNode*>())
            : return_type_name_(return_type_name)
            , method_name_(method_name)
            , args_(args)
    {}

    const std::string& MethodName() const {
        return method_name_;
    }

    const std::string& ReturnTypeName() const {
        return return_type_name_;
    }

    const std::vector<BaseNode*>& Arguments() const {
        return args_;
    }

    void Visit(BaseVisitor* visitor) const override {
        visitor->Visit(this);
    }

  private:
    std::string return_type_name_;
    std::string method_name_;
    std::vector<BaseNode*> args_;
};

#include "format_visitor.h"

std::vector<std::string> expected_code = {
"class MyClass {",
"  public:",
"    void Print();",
"    void SetNum(int n);",
"",
"  protected:",
"    OtherClass SomeMethod(int a, char b, MyClass c);",
"    int x_;",
"",
"  private:",
"    void SetNumImpl(int num);",
"    class Iterator {",
"      public:",
"        void Next();",
"        bool Stopped();",
"        int* Dereference();",
"    };",
"};",
};


int main() {
    auto n_decl = std::make_unique<VarDeclarationNode>("n", "int");
    auto x_decl = std::make_unique<VarDeclarationNode>("x_", "int");
    auto a_decl = std::make_unique<VarDeclarationNode>("a", "int");
    auto b_decl = std::make_unique<VarDeclarationNode>("b", "char");
    auto c_decl = std::make_unique<VarDeclarationNode>("c", "MyClass");
    auto num_decl = std::make_unique<VarDeclarationNode>("num", "int");

    auto print = std::make_unique<MethodDeclarationNode>("void", "Print");
    std::unique_ptr<MethodDeclarationNode> set_num(
        new MethodDeclarationNode("void", "SetNum", {n_decl.get()}));

    std::unique_ptr<MethodDeclarationNode> some_method(new MethodDeclarationNode(
        "OtherClass", "SomeMethod", {a_decl.get(), b_decl.get(), c_decl.get()}));

    std::unique_ptr<MethodDeclarationNode> set_num_impl(
        new MethodDeclarationNode("void", "SetNumImpl", {num_decl.get()}));

    auto next = std::make_unique<MethodDeclarationNode>("void", "Next");
    auto stopped = std::make_unique<MethodDeclarationNode>("bool", "Stopped");
    auto dereference = std::make_unique<MethodDeclarationNode>("int*", "Dereference");

    std::unique_ptr<ClassDeclarationNode> iterator(
        new ClassDeclarationNode("Iterator", {next.get(), stopped.get(), dereference.get()}));

    std::unique_ptr<ClassDeclarationNode> my_class(
        new ClassDeclarationNode("MyClass",
        {print.get(), set_num.get()}, {some_method.get(), x_decl.get()}, {set_num_impl.get(), iterator.get()}));

    FormatVisitor fv;
    fv.Visit(my_class.get());
    auto formatted = fv.GetFormattedCode();
    
    assert(expected_code == formatted);
    
    std::cout << 1 << std::endl;
}