#include "CodeGenVisitor.h"
#include <iostream>
#include <unistd.h>

const bool VERBOSE_DEBUG_OUTPUT = false;

llvm::Type* CodeGenVisitor::TypeToLLVMType(Type t) {
  if (t == Type::INT_TYPE) {
    return llvm::Type::getInt32Ty(context);
  } else if (t == Type::BOOLEAN_TYPE) {
    return llvm::Type::getInt1Ty(context);
  } else if (t == Type::VOID_TYPE) {
    return llvm::Type::getVoidTy(context);
  } else {
    std::cerr << "UNHANDLED TYPE!\n";
  }
}

llvm::Constant* CodeGenVisitor::TypeToDefaultValue(Type type) {
  if (type != Type::INT_TYPE && type != Type::BOOLEAN_TYPE) {
    std::cerr << "WARNING: no default value exists for non-int non-bool types.\n";
  }
  return llvm::ConstantInt::get(context,
    llvm::APInt(type == Type::INT_TYPE ? 32 : 1, 0, true));
}

using namespace llvm;

CodeGenVisitor::CodeGenVisitor()
: builder(context)
, module(std::move(llvm::make_unique<Module>("module", context)))
, for_internal_bb(nullptr) 
, for_after_bb(nullptr) { }

void CodeGenVisitor::AnnulReturnWithError(const std::string& error) {
  std::cerr << error << "Returning nullptr.\n";
  ret = nullptr;
}

Function* CodeGenVisitor::CurrentFunction() {
  return builder.GetInsertBlock()->getParent();
}

bool CodeGenVisitor::CurrentBlockDone() {
  return builder.GetInsertBlock()->getTerminator();
}

Constant* CodeGenVisitor::GetConstIntN(unsigned N, int value) {
  return ConstantInt::get(
    llvm::Type::getIntNTy(context, N), APInt(N, value, true));
}

// bool CodeGenVisitor::Ensure(bool predicate) {
//   if (!predicate) {
//     success = false;
//   }
//   return predicate;
// }

// void CodeGenVisitor::ForceSame(Type& t, Type u) {
//   if (t == Type::UNKNOWN) {
//     t = u;
//   } else {
//     Ensure(t == u);
//   }
// }

// void DumpVar(Var v) {
//   std:: cerr << TypeToString(v.type) << ' ' << v.id << "; ";
// }

// void CodeGenVisitor::DumpVars() {
//   std::cerr << "~~~~VARS~~~~\n";
//   for (auto pr : vars) {
//     std::cerr << TypeToString(pr.second.first) << (pr.second.second ? "[]" : "") << ' ' << pr.first << '\n';

//   }
// }

Value* CodeGenVisitor::GEPFromLocationNode(LocationNode *node) {
  if (node->index == nullptr) {
    return vars[node->id];
  } else {
    Value* index_value; node->index->accept(this); index_value = ret;
    if (!index_value) {
      std::cerr << "index value is null!\nreturning nullptr.\n";
      return nullptr;
    }
    return builder.CreateGEP(vars[node->id], ArrayRef<Value *>({GetConstIntN(32, 0), index_value}), "indexed location");
  }
}

void CodeGenVisitor::AddScopedVar(const std::string& name, Value* alloca, VarTable& shadow_list) {
  if (vars.count(name) > 0) {
    // two params with same name
    if (shadow_list.count(name) == 0) {
      shadow_list[name] = vars[name]; // shadowing
    }
  } else {
    // store a placeholder so we know to remove the id later
    shadow_list[name] = nullptr;
  }
  vars[name] = alloca;
}
void CodeGenVisitor::RestoreShadowedVars(const VarTable& shadow_list) {
  for (auto pr : shadow_list) {
    if (VERBOSE_DEBUG_OUTPUT) std::cerr << "Restoring " << pr.first << '\n';
    if (pr.second == nullptr) {
      vars.erase(pr.first);
    } else {
      vars[pr.first] = pr.second; // unshadow old variables
    }
  }
}

AllocaInst *CreateEntryBlockAlloca(Function *fn, llvm::Type* type, const std::string &id, Value* array_size = nullptr) {
  IRBuilder<> temp_builder(&fn->getEntryBlock(),
                 fn->getEntryBlock().begin());
  return temp_builder.CreateAlloca(type, nullptr,
                           id.c_str());
}

void CodeGenVisitor::visit(IntLitNode* node) {
  ret = ConstantInt::get(context, APInt(32, node->value, true));
}
void CodeGenVisitor::visit(CharLitNode* node) {
  ret = ConstantInt::get(context, APInt(8, node->value, false));
}
void CodeGenVisitor::visit(StringLitNode* node) {
  ret = builder.CreateGlobalStringPtr(StringRef(node->value));
}
void CodeGenVisitor::visit(BoolLitNode* node) {
  ret = ConstantInt::get(context, APInt(1, node->value, true));
}
void CodeGenVisitor::visit(LocationNode* node) {
  Value* elem_ptr = GEPFromLocationNode(node);
  if (!elem_ptr) {
    AnnulReturnWithError("elem_ptr is null!\n");
    return;
  }
  ret = builder.CreateLoad(elem_ptr, node->id + "[]");
}
void CodeGenVisitor::visit(MethodCallNode* node) {
  if (node->callout_args.empty()) {
    Function *called_fn = module->getFunction(node->id);
    if (!called_fn) {
      AnnulReturnWithError("Unknown function :(\n");
      return;
    }
    std::vector<Value*> args;
    for (auto arg_node : node->args) {
      arg_node->accept(this);
      if (!ret) {
        AnnulReturnWithError("Arg node could not be codegen'd\n");
        return;
      }
      args.push_back(ret);
    }
    ret = builder.CreateCall(called_fn, args, "call");
  } else {
    // TODO: HANDLE CALLOUT
  }
}
void CodeGenVisitor::visit(BinopNode* node) {
  Value *val_left; node->left->accept(this); val_left = ret;
  Value *val_right; node->right->accept(this); val_right = ret;
  if (!val_left || !val_right) {
    AnnulReturnWithError("Binop child gave nullptr value!\n");
    return;
  }

  switch (node->op) {
    case Op::PLUS:
      ret = builder.CreateAdd(val_left, val_right, "binop_add");
      break;
    case Op::MINUS:
      ret = builder.CreateSub(val_left, val_right, "binop_sub");
      break;
    case Op::MULT:
      ret = builder.CreateMul(val_left, val_right, "binop_mult");
      break;
    case Op::DIV:
      ret = builder.CreateSDiv(val_left, val_right, "binop_div");
      break;
    case Op::MOD:
      ret = builder.CreateSRem(val_left, val_right, "binop_mod");
      break;
    case Op::EQ:
      ret = builder.CreateICmpEQ(val_left, val_right, "binop_eq");
      break;
    case Op::NE:
      ret = builder.CreateICmpNE(val_left, val_right, "binop_ne");
      break;
    case Op::LT:
      ret = builder.CreateICmpSLT(val_left, val_right, "binop_lt");
      break;
    case Op::LE:
      ret = builder.CreateICmpSLE(val_left, val_right, "binop_le");
      break;
    case Op::GT:
      ret = builder.CreateICmpSGT(val_left, val_right, "binop_gt");
      break;
    case Op::GE:
      ret = builder.CreateICmpSGE(val_left, val_right, "binop_ge");
      break;
    case Op::OR:
      ret = builder.CreateOr(val_left, val_right, "binop_or");
      break;
    case Op::AND:
      ret = builder.CreateAnd(val_left, val_right, "binop_and");
      break;
  }
}
void CodeGenVisitor::visit(UnopNode* node) {
  Value *val; node->expr->accept(this); val = ret;

  switch(node->op) {
    case Op::MINUS:
      ret = builder.CreateNeg(val, "unop_neg");
      break;
    case Op::NOT:
      ret = builder.CreateNot(val, "unop_not");
      break;
  }
}
void CodeGenVisitor::visit(BlockNode* node) {
  VarTable shadow_list;
  for (Var v : node->var_decls) {
    AllocaInst *alloca = CreateEntryBlockAlloca(CurrentFunction(),
      TypeToLLVMType(v.type), v.id);
    AddScopedVar(v.id, alloca, shadow_list);
    builder.CreateStore(TypeToDefaultValue(v.type), alloca);
  }
  for (auto statement : node->statements) {
    statement->accept(this);
    if (CurrentBlockDone()) {
      break; // no code paths to this location.
    }
  }
  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(AssignNode* node) {
  Value *value; node->value->accept(this); value = ret;
  Value *elem_ptr = GEPFromLocationNode(node->location);
  if (node->op != AssignOp::ASSIGN) {
    Value *cur_value = builder.CreateLoad(elem_ptr, node->location->id);
    if (node->op == AssignOp::PLUS_ASSIGN) {
      value = builder.CreateAdd(cur_value, value, "plus_assign");
    } else {
      value = builder.CreateSub(cur_value, value, "minus_assign");
    }
  }
  builder.CreateStore(value, elem_ptr);
}
void CodeGenVisitor::visit(IfNode* node) {
  Value *cond_value; node->cond->accept(this); cond_value = ret;
  if (!cond_value) {
    AnnulReturnWithError("Cond is null!\n");
    return;
  }

  BasicBlock *then_bb = BasicBlock::Create(context, "then", CurrentFunction());
  BasicBlock *merge_bb = BasicBlock::Create(context, "if_then_merge");
  BasicBlock *else_bb = (node->otherwise != nullptr) ?
                        BasicBlock::Create(context, "else") :
                        merge_bb;
  builder.CreateCondBr(cond_value, then_bb, else_bb);
  builder.SetInsertPoint(then_bb);

  Value* then_value; node->then->accept(this); then_value = ret;
  if (!then_value) {
    AnnulReturnWithError("Then value is null!\n");
    return;
  }
  if (!CurrentBlockDone()) {
    builder.CreateBr(merge_bb);
  }

  if (node->otherwise != nullptr) {
    CurrentFunction()->getBasicBlockList().push_back(else_bb);
    builder.SetInsertPoint(else_bb);
    Value* else_value; node->otherwise->accept(this); else_value = ret;
    if (!else_value) {
      AnnulReturnWithError("else value is null!\n");
      return;
    }
    if (!CurrentBlockDone()) {
      builder.CreateBr(merge_bb);
    }
  }

  if (merge_bb->hasNUsesOrMore(1)) {
    CurrentFunction()->getBasicBlockList().push_back(merge_bb);
    builder.SetInsertPoint(merge_bb);
  }
}
void CodeGenVisitor::visit(ForNode* node) {
  Value* start_value; node->start->accept(this); start_value = ret;
  if (!start_value) {
    AnnulReturnWithError("start value is null!\n");
    return;
  }

  Value* end_value; node->end->accept(this); end_value = ret;
  if (!end_value) {
    AnnulReturnWithError("end value is null!\n");
    return;
  }

  bool always_enter_loop = false;
  Value *brcond = builder.CreateICmpSLT(start_value, end_value);
  if (ConstantInt * c = dyn_cast<ConstantInt>(brcond)) {
    if (c->isZero()) {
      return;
    } else {
      always_enter_loop = true;
    }
  };

  BasicBlock *old_for_internal_bb = for_internal_bb;
  BasicBlock *old_for_after_bb = for_after_bb;
  BasicBlock *for_bb = BasicBlock::Create(context, "for " + node->id, CurrentFunction());
  for_internal_bb = BasicBlock::Create(context, "for_internal " + node->id);
  for_after_bb = BasicBlock::Create(context, "for_after " + node->id);

  VarTable shadow_list;
  AllocaInst* i_alloca = CreateEntryBlockAlloca(CurrentFunction(), llvm::Type::getInt32Ty(context), node->id);
  AddScopedVar(node->id, i_alloca, shadow_list);
  builder.CreateStore(start_value, i_alloca);
  
  if (always_enter_loop) {
    builder.CreateBr(for_bb);
  } else {
    builder.CreateCondBr(brcond, for_bb, for_after_bb);
  }

  builder.SetInsertPoint(for_bb);
  
  Value *body_value; node->body->accept(this); ret = body_value;
  if (!body_value) {
    AnnulReturnWithError("body value is null!\n");
    return;
  }

  if (!CurrentBlockDone()) {
    builder.CreateBr(for_internal_bb);
  }
  if (for_internal_bb->hasNUsesOrMore(1)) {
    CurrentFunction()->getBasicBlockList().push_back(for_internal_bb);
    builder.SetInsertPoint(for_internal_bb);
    Value* i_val = builder.CreateAdd(builder.CreateLoad(i_alloca),
                                    GetConstIntN(32, 1));
    builder.CreateStore(i_val, i_alloca);
    builder.CreateCondBr(builder.CreateICmpSLT(i_val, end_value),
                      for_bb, for_after_bb);
  }
  if (for_after_bb->hasNUsesOrMore(1)) {
    CurrentFunction()->getBasicBlockList().push_back(for_after_bb);
    builder.SetInsertPoint(for_after_bb);
  }

  for_internal_bb = old_for_internal_bb;
  for_after_bb = old_for_after_bb;
  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(ReturnNode* node) {
  // std::cerr << "return node\n";
  if (node->value != nullptr) {
    Value *value; node->value->accept(this); value = ret;
    builder.CreateRet(value);
  } else {
    builder.CreateRetVoid();
  }
}
void CodeGenVisitor::visit(LoopControlNode* node) {
  if (for_internal_bb == nullptr) {
    AnnulReturnWithError("Loop control operation occured outside any loop!\n");
    return;
  }
  switch (node->type) {
    case LoopControlType::CONTINUE:
      builder.CreateBr(for_internal_bb);
      break;
    case LoopControlType::BREAK:
      builder.CreateBr(for_after_bb);
      break;
  }
}
void CodeGenVisitor::visit(MethodNode* node) {
  std::vector<llvm::Type*> param_types;
  // std::cerr << "Function arguments:\n";
  for (Var v : node->params) {
    param_types.push_back(TypeToLLVMType(v.type));
    // std::cerr << TypeToString(v.type) << ' ' ;
  } // std::cerr << '\n';
  FunctionType *prototype = FunctionType::get(
    TypeToLLVMType(node->return_type),
    param_types,
    false);
  Function *fn = Function::Create(
    prototype, Function::ExternalLinkage, node->id, module.get());
  if (!fn) {
    AnnulReturnWithError("Function prototype has nullptr value.\n");
    return;
  }

  BasicBlock *bb = BasicBlock::Create(context, "entry", fn);
  builder.SetInsertPoint(bb);
  
  VarTable shadow_list;
  int idx = 0;
  for (auto &arg : fn->args()) {
    // std::cerr << "node->param is " << node->params[idx].id << '\n';
    arg.setName(node->params[idx].id);
    AllocaInst *alloca = CreateEntryBlockAlloca(fn,
      prototype->getParamType(idx), arg.getName());
    builder.CreateStore(&arg, alloca);
    AddScopedVar(node->params[idx].id, alloca, shadow_list);
    idx++;
  }

  node->body->accept(this);
  // needed because every basic block *must* have a terminator
  // the return statement has no semantic effect.
  if (!CurrentBlockDone() && node->IsVoid()) {
    builder.CreateRetVoid();
  }
  // else {
  //   builder.CreateRet(TypeToDefaultValue(node->return_type));
  // }
  verifyFunction(*fn, &errs());
  // fn->print(errs());
  // ret = fn;

  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(RootNode* node) {
  VarTable shadow_list;
  for (Field f : node->field_decls) {
    int length = (f.length == -1 ? 1 : f.length);
    llvm::Type *type = TypeToLLVMType(f.type);
    GlobalVariable *ptr;
    if (length == 1) {
      ptr = (GlobalVariable*) module->getOrInsertGlobal(f.id, type);
      ptr->setInitializer(TypeToDefaultValue(f.type));
    } else {
      llvm::ArrayType *array_type = ArrayType::get(type, length);
      ptr = (GlobalVariable*) module->getOrInsertGlobal(f.id, array_type);
      ptr->setInitializer(ConstantArray::get(array_type,
        ArrayRef(std::vector<Constant*>(length, TypeToDefaultValue(f.type)))));
    }
    AddScopedVar(f.id, ptr, shadow_list);
  }

  for (MethodNode *method : node->method_decls) {
    method->accept(this);
  }
  module->print(outs(), nullptr);

  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(UnvisitableNode* node) { }
void CodeGenVisitor::visit(TypeNode* node) { }
