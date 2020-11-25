#pragma once
#include <string>
#include <vector>

class FormatVisitor : public BaseVisitor {
 public:
     void Visit(const BaseNode* node) override {
         node->Visit(this);
     }

     void Visit(const ClassDeclarationNode* node) override {
         ++lvl;
         std::string cl_sp(4 * lvl, ' ');
         code.push_back(cl_sp + "class " + node->ClassName() + " {");
         //
         if (node->PublicFields().size()) {
             code.push_back(cl_sp + "  " + "public:");
             for (auto pbl_field : node->PublicFields()) {
                 this->Visit(pbl_field);
             }
             if (node->ProtectedFields().size() ||
                 node->PrivateFields().size()) {
                 code.push_back("");
             }
         }
         //
         if (node->ProtectedFields().size()) {
             code.push_back(cl_sp + "  " + "protected:");
             for (auto prt_field : node->ProtectedFields()) {
                 this->Visit(prt_field);
             }
             if (node->PrivateFields().size()) {
                 code.push_back("");
             }
         }
         //
         if (node->PrivateFields().size()) {
             code.push_back(cl_sp + "  " + "private:");
             for (auto pvt_field : node->PrivateFields()) {
                 this->Visit(pvt_field);
             }
         }
         code.push_back(cl_sp + "};");
         lvl--;
     };
     void Visit(const VarDeclarationNode* node) override {
         var = node->TypeName() + " " + node->VarName();
         if (isVar) {
             std::string vr_sp(4 * (lvl + 1), ' ');
             code.push_back(vr_sp + var + ";");
         }
     };
     void Visit(const MethodDeclarationNode* node) override {
         isVar = false;
         std::string vr_sp(4 * (lvl + 1), ' ');
         method = node->ReturnTypeName() + " " + node->MethodName() + "(";
         for (unsigned int i = 0; i < node->Arguments().size(); i++) {
             this->Visit(node->Arguments()[i]);
             method += var;
             if (i != node->Arguments().size() - 1) {
                 method += ", ";
             }
         }
         code.push_back(vr_sp + method + ");");
         isVar = true;
     };

     const std::vector<std::string>& GetFormattedCode() const {
         return code;
     }

 private:
     std::vector<std::string> code;
     std::string add;
     std::string method;
     std::string var;
     bool isVar = true;
     int lvl = -1;
};
