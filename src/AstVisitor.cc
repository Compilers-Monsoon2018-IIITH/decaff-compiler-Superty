#include "AstNode.h"
#include "BlockNode.h"
#include "ExprNode.h"
#include "RootNode.h"
#include "AstVisitor.h"

void MethodNode::accept(AstVisitor* v) {
  v->visit(this);
}
void RootNode::accept(AstVisitor* v) {
  v->visit(this);
}
void TypeNode::accept(AstVisitor* v) {
  v->visit(this);
}
void BlockNode::accept(AstVisitor* v) {
  v->visit(this);
}
void AssignNode::accept(AstVisitor* v) {
  v->visit(this);
}
void IfNode::accept(AstVisitor* v) {
  v->visit(this);
}
void ForNode::accept(AstVisitor* v) {
  v->visit(this);
}
void ReturnNode::accept(AstVisitor* v) {
  v->visit(this);
}
void LoopControlNode::accept(AstVisitor* v) {
  v->visit(this);
}
void LocationNode::accept(AstVisitor* v) {
  v->visit(this);
}
void MethodCallNode::accept(AstVisitor* v) {
  v->visit(this);
}
void IntLitNode::accept(AstVisitor* v) {
  v->visit(this);
}
void CharLitNode::accept(AstVisitor* v) {
  v->visit(this);
}
void StringLitNode::accept(AstVisitor* v) {
  v->visit(this);
}
void BoolLitNode::accept(AstVisitor* v) {
  v->visit(this);
}
void BinopNode::accept(AstVisitor* v) {
  v->visit(this);
}
void UnopNode::accept(AstVisitor* v) {
  v->visit(this);
}
// void AstNode::accept(AstVisitor* v) {
//   v->visit(this);
// }
