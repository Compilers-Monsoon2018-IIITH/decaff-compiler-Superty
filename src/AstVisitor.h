#pragma once

class MethodNode;
class RootNode;
class TypeNode;
class BlockNode;
class AssignNode;
class IfNode;
class ForNode;
class ReturnNode;
class LoopControlNode;
class LocationNode;
class MethodCallNode;
class IntLitNode;
class CharLitNode;
class StringLitNode;
class BoolLitNode;
class BinopNode;
class UnopNode;
class UnvisitableNode;
class AstNode;

class AstVisitor {
public:
  virtual void visit(IntLitNode* node) = 0;
  virtual void visit(CharLitNode* node) = 0;
  virtual void visit(StringLitNode* node) = 0;
  virtual void visit(BoolLitNode* node) = 0;
  virtual void visit(LocationNode* node) = 0;
  virtual void visit(MethodCallNode* node) = 0;
  virtual void visit(BinopNode* node) = 0;
  virtual void visit(UnopNode* node) = 0;
  virtual void visit(TypeNode* node) = 0;
  virtual void visit(BlockNode* node) = 0;
  virtual void visit(AssignNode* node) = 0;
  virtual void visit(IfNode* node) = 0;
  virtual void visit(ForNode* node) = 0;
  virtual void visit(ReturnNode* node) = 0;
  virtual void visit(LoopControlNode* node) = 0;
  virtual void visit(MethodNode* node) = 0;
  virtual void visit(RootNode* node) = 0;
  virtual void visit(UnvisitableNode* node) = 0;
};
