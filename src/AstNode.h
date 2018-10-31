#pragma once

class AstNode;

class AstVisitor {
public:
  virtual void visit(AstNode* node) = 0;
};

class AstNode {
public:
  virtual ~AstNode() = 0;
  virtual void accept(AstVisitor* v) = 0;
};
