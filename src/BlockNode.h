#pragma once

#include "ExprNode.h"

using IdList = ListNode<StringLitNode*>;

struct Var {
  Type type;
  std::string id;
};
using VarList = ListNode<Var>;

class TypeNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  Type type;
  TypeNode(Type o_type);
};
Type ReduceToType(TypeNode *node);
Var ReduceToVar(AstNode *type, AstNode *id);

enum class StatementType {
  ASSIGN, CALL, IF, FOR, RETURN, BREAK, CONTINUE, BLOCK
};
enum class AssignOp {
  ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN
};

class BlockNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  std::vector<Var> var_decls;
  std::vector<AstNode*> statements;
  void AppendVars(TypeNode* type, IdList* ids);
  void SetStatements(ListNode<AstNode*> *o_statements);
};
class AssignNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  LocationNode *location;
  AssignOp op;
  AstNode *value;
  AssignNode(LocationNode *o_location, AssignOp o_op, AstNode *o_value);
};
class IfNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  AstNode *cond;
  BlockNode *then, *otherwise;
  IfNode(AstNode *cond, BlockNode *then, BlockNode *otherwise);
};
class ForNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  std::string id;
  AstNode *start, *end;
  BlockNode *body;
  ForNode(std::string o_id, AstNode *o_start, AstNode *o_end, BlockNode *o_body);
};
class ReturnNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  AstNode *value;
  ReturnNode(AstNode *o_value);
};

enum class LoopControlType {
  CONTINUE, BREAK
};
class LoopControlNode : public AstNode {
public:
  void accept(AstVisitor* v) override;
  LoopControlType type;
  LoopControlNode(LoopControlType o_type);
};