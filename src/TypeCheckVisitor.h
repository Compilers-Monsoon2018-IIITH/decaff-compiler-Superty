#include "AstVisitor.h"
#include <unordered_map>
#include "AstNode.h"
#include "ExprNode.h"
#include "BlockNode.h"
#include "RootNode.h"

using VarTable = std::unordered_map<std::string,std::pair<Type,bool>>;
// the bool indicates whether the type is an array type

class TypeCheckVisitor : public AstVisitor {
public:
  void visit(IntLitNode* node) override;
  void visit(CharLitNode* node) override;
  void visit(StringLitNode* node) override;
  void visit(BoolLitNode* node) override;
  void visit(LocationNode* node) override;
  void visit(MethodCallNode* node) override;
  void visit(BinopNode* node) override;
  void visit(UnopNode* node) override;
  void visit(TypeNode* node) override;
  void visit(BlockNode* node) override;
  void visit(AssignNode* node) override;
  void visit(IfNode* node) override;
  void visit(ForNode* node) override;
  void visit(ReturnNode* node) override;
  void visit(LoopControlNode* node) override;
  void visit(MethodNode* node) override;
  void visit(RootNode* node) override;
  void visit(UnvisitableNode* node) override;
  bool success;
  TypeCheckVisitor();
private:
  VarTable vars;
  std::unordered_map<std::string,std::vector<Type>> methods;

  void AddScopedVar(Var var, VarTable& shadow_list);
  void RestoreShadowedVars(const VarTable& shadow_list);
  void DumpVars();
  bool Ensure(bool predicate);
  void ForceSame(Type& t, Type u);
};
