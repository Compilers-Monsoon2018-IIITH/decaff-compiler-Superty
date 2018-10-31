#pragma once

#include "BlockNode.h"

#include <vector>

struct Field {
  Type type;
  std::string id;
  int length;
};

using FieldList = ListNode<Field>;

class MethodNode : public AstNode {
public:
  Type return_type;
  std::string id;
  std::vector<Var> params;
  BlockNode *body;
  void accept(AstVisitor* v) override;
  MethodNode(Type o_return_type, std::string o_id, VarList *var_list, BlockNode *o_body);
};

MethodNode* ConstructMethodNode(AstNode* ret, AstNode* id, AstNode* var_list, AstNode* body);

class RootNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  std::vector<Field> field_decls;
  std::vector<MethodNode*> method_decls;
  std::string id;
  void AppendFields(TypeNode* type, FieldList* ids);
};
