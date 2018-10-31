#include "ExprNode.h"

void LocationNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void MethodCallNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void IntLitNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void CharLitNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void StringLitNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void BoolLitNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void BinopNode::accept(AstVisitor* v) override {
  v->visit(this);
}
void UnopNode::accept(AstVisitor* v) override {
  v->visit(this);
}