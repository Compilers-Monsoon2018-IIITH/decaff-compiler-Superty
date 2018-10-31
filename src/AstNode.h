#pragma once

enum class Type {
  CHAR_TYPE, INT_TYPE, BOOLEAN_TYPE, STRING_TYPE, VOID_TYPE, ERROR
};

class AstVisitor;

class AstNode {
public:
  virtual ~AstNode();
  virtual void accept(AstVisitor* v) = 0;
  Type t;
};

class UnvisitableNode : public AstNode {
  void accept(AstVisitor* v) override;
};