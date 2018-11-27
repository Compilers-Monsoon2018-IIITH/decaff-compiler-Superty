#pragma once

#include "AstNode.h"
#include "ListNode.h"
#include <string>
#include <vector>
#include <variant>

using CalloutArg = std::variant<AstNode*, std::string>;
using CalloutArgList = ListNode<CalloutArg>;
using ArgList = ListNode<AstNode*>;

class IntLitNode : public AstNode {
public:
  int value;
  void accept(AstVisitor* v) override;
  IntLitNode(int o_value);
};
class CharLitNode : public AstNode {
public:
  char value;
  void accept(AstVisitor* v) override;
  CharLitNode(char o_value);
};
class StringLitNode : public AstNode {
public:
  std::string value;
  void accept(AstVisitor* v) override;
  StringLitNode(std::string o_value);
};
class BoolLitNode : public AstNode {
public:
  bool value;
  void accept(AstVisitor* v) override;
  BoolLitNode(bool o_value);
};

enum class Op {
  OR, AND, EQ, NE, LT, LE, GE, GT, PLUS, MINUS, MULT, DIV, MOD, NOT
};
std::string OpToString(Op op);

class LocationNode : public AstNode {
public:
  std::string id;
  AstNode* index;
  void accept(AstVisitor* v) override;
  LocationNode(StringLitNode *o_id, AstNode* o_index = nullptr);
};
class MethodCallNode : public AstNode {
public:
  std::string id;
  std::vector<AstNode*> args;
  std::vector<CalloutArg> callout_args;
  bool is_callout;
  void accept(AstVisitor* v) override;
  MethodCallNode(StringLitNode *o_id, CalloutArgList* o_callout_args);
  MethodCallNode(StringLitNode *o_id, ArgList* o_args);
};
class BinopNode : public AstNode {
public:
  Op op;
  AstNode *left, *right;
  void accept(AstVisitor* v) override;
  BinopNode(AstNode* o_left, Op o_op, AstNode *o_right);
};
class UnopNode : public AstNode {
public:
  Op op;
  AstNode *expr;
  void accept(AstVisitor* v) override;
  UnopNode(Op o_op, AstNode* o_expr);
};
std::string ReduceToString(StringLitNode* node);
int ReduceToInt(IntLitNode* node);
