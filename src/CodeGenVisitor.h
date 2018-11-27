#include "AstVisitor.h"
#include <unordered_map>
#include "AstNode.h"
#include "ExprNode.h"
#include "BlockNode.h"
#include "RootNode.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

// using namespace llvm;

class CodeGenVisitor : public AstVisitor {
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
  CodeGenVisitor();
private:
  using VarTable = std::unordered_map<std::string,llvm::Value*>;

  void AnnulReturnWithError(const std::string& error);
  bool CurrentBlockDone();
  llvm::Value* GEPFromLocationNode(LocationNode *node);

  llvm::Constant *GetConstIntN(unsigned N, int value);
  llvm::Constant *TypeToDefaultValue(Type type);
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::Module> module;
  llvm::Value *ret;
  VarTable vars;
  llvm::Function* CurrentFunction();
  // std::unordered_map<std::string,std::vector<Type>> methods;

  void AddScopedVar(const std::string& name,
    llvm::Value* alloca, VarTable& shadow_list);
  void RestoreShadowedVars(const VarTable& shadow_list);
  llvm::Type* TypeToLLVMType(Type t);
  // void DumpVars();
  // bool Ensure(bool predicate);
  // void ForceSame(Type& t, Type u);

  // these are the blocks corresponding to the innermost for loop we are in.
  // set to nullptr, if we aren't in a for loop.
  llvm::BasicBlock *for_internal_bb, *for_after_bb;
};
