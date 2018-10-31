#pragma once

#include "AstNode.h"
#include <string>
#include <vector>

struct CalloutArg {
  AstNode *maybe_expr;
  std::string maybe_string;
};
struct Location {
  std::string id;
  AstNode *index;
};
enum class BinOp {
OR, AND, EQ, NE, LT, LE, GE, GT, PLUS, MINUS, MULT, DIV, MOD };
enum class UnOp {
  NEG, NOT
};

class LocationNode : public AstNode {
public:
  Location location;
  void accept(AstVisitor* v) override;
};
class MethodCallNode : public AstNode {
public:
  std::string id;
  std::vector<AstNode*> args;
  std::vector<CalloutArg> callout_args;
  void accept(AstVisitor* v) override;
};
class IntLitNode : public AstNode {
public:
  int value;
  void accept(AstVisitor* v) override;
};
class CharLitNode : public AstNode {
public:
  char value;
  void accept(AstVisitor* v) override;
};
class StringLitNode : public AstNode {
public:
  std::string value;
  void accept(AstVisitor* v) override;
};
class BoolLitNode : public AstNode {
public:
  bool value;
  void accept(AstVisitor* v) override;
};
class BinopNode : public AstNode {
public:
  BinOp op;
  AstNode *left, *right;
  void accept(AstVisitor* v) override;
};
class UnopNode : public AstNode {
public:
  UnOp op;
  AstNode *expr;
  void accept(AstVisitor* v) override;
};
