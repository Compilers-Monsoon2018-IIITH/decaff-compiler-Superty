#pragma once

enum class Type {
  INT_TYPE, BOOLEAN_TYPE, STRING_TYPE, VOID_TYPE
};

class AstVisitor;

class AstNode {
public:
  virtual ~AstNode();
  virtual void accept(AstVisitor* v) = 0;
  Type t;
};
