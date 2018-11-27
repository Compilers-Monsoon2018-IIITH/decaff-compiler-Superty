#include "CodeGenVisitor.h"
#include <iostream>
#include <unistd.h>

const bool VERBOSE_DEBUG_OUTPUT = false;
const char* NONCALLOUT_PREFIX = "noncallout_";

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

Value* CodeGenVisitor::GetCallout(const std::string& id, llvm::FunctionType *prototype, std::vector<Value*> args) {
  Function *fn = module->getFunction(id);
  if (!prototype) {
    std::cerr << "could not create prototype\n returning nullptr\n";
    return nullptr;
  }
  if (!fn) {
    fn = Function::Create(prototype, Function::ExternalLinkage, id, module.get());
  }
  Value *callee = fn;
  if (fn->getFunctionType() != prototype) {
    callee = builder.CreateBitCast(fn, PointerType::get(prototype, 0), "BIT_CAST_CALLOUT");
  }

  if (!callee) {
    std::cerr << "could not get callee\n returning nullptr\n";
    return nullptr;
  }

  return builder.CreateCall(callee, args);
}

void CodeGenVisitor::GenErrorAndExitInst(const std::string& error) {
  FunctionType *exit_prototype = FunctionType::get(
    llvm::Type::getVoidTy(context),
    {llvm::Type::getInt32Ty(context)},
    false
  );
  FunctionType *printf_prototype = FunctionType::get(
    llvm::Type::getInt32Ty(context),
    {llvm::Type::getInt8PtrTy(context)},
    false
  );
  Value* error_string = builder.CreateGlobalStringPtr(error);
  GetCallout("printf", printf_prototype, {error_string});
  GetCallout("exit", exit_prototype, {GetConstIntN(32, 1)});
  llvm::Type* return_type = CurrentFunction()->getReturnType();
  if (return_type->isVoidTy()) {
    builder.CreateRetVoid();
  } else {
    builder.CreateRet(Constant::getNullValue(return_type));
  }
}

Value* CodeGenVisitor::GEPFromLocationNode(LocationNode *node) {
  if (node->index == nullptr) {
    if (vars[node->id].second->isArrayTy()) {
      return builder.CreateGEP(vars[node->id].first, ArrayRef<Value *>({GetConstIntN(32, 0), GetConstIntN(32, 0)}), "pointer_to_start_of_array");
    } else {
      return vars[node->id].first;
    }
  } else {
    Value* index_value; node->index->accept(this); index_value = ret;
    if (!index_value) {
      std::cerr << "index value is null!\nreturning nullptr.\n";
      return nullptr;
    }

    Value *check_lt_length = builder.CreateICmpSLT(index_value, GetConstIntN(32, array_lengths[node->id]), "gep_check_lt_length");
    Value *check_nonneg = builder.CreateICmpSGE(index_value, GetConstIntN(32, 0), "gep_check_nonneg");
    Value *cond_value = builder.CreateAnd(check_lt_length, check_nonneg, "gep_bounds_check");
    if (!cond_value) {
      std::cerr << "gep_bounds_check val is null\n returning nullptr\n";
      return nullptr;
    }

    BasicBlock *out_of_bounds_bb = BasicBlock::Create(context, "gep_bounds_check_fail", CurrentFunction());
    BasicBlock *bounds_check_success_bb = BasicBlock::Create(context, "gep_succesful_bounds_check");
    builder.CreateCondBr(cond_value, bounds_check_success_bb, out_of_bounds_bb);
    builder.SetInsertPoint(out_of_bounds_bb);
    GenErrorAndExitInst("\nruntime error: array access to " + node->id + " out of bounds.\n");

    CurrentFunction()->getBasicBlockList().push_back(bounds_check_success_bb);
    builder.SetInsertPoint(bounds_check_success_bb);
    return builder.CreateGEP(vars[node->id].first, ArrayRef<Value *>({GetConstIntN(32, 0), index_value}), "indexed location");
  }
}

void CodeGenVisitor::AddScopedVar(const std::string& name, std::pair<Value*,llvm::Type*> alloca, VarTable& shadow_list) {
  if (vars.count(name) > 0) {
    // two params with same name
    if (shadow_list.count(name) == 0) {
      shadow_list[name] = vars[name]; // shadowing
    }
  } else {
    // store a placeholder so we know to remove the id later
    shadow_list[name] = {nullptr, nullptr};
  }
  vars[name] = alloca;
}
void CodeGenVisitor::RestoreShadowedVars(const VarTable& shadow_list) {
  for (auto pr : shadow_list) {
    if (VERBOSE_DEBUG_OUTPUT) std::cerr << "Restoring " << pr.first << '\n';
    if (pr.second.first == nullptr && pr.second.second == nullptr) {
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
  if (vars[node->id].second->isArrayTy() && node->index == nullptr) {
    ret = elem_ptr;
  } else {
    ret = builder.CreateLoad(elem_ptr, node->id + (node->index != nullptr ? "[]" : ""));
  }
}
void CodeGenVisitor::visit(MethodCallNode* node) {
  if (!node->is_callout) {
    Function *called_fn = module->getFunction(node->id);
    if (!called_fn) {
      AnnulReturnWithError("Unknown function " + node->id + "\n");
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
    if (called_fn->getReturnType()->isVoidTy()) {
      // CallInst::Create(called_fn, args, builder.GetInsertBlock());
      builder.CreateCall(called_fn, args);
    } else {
      ret = builder.CreateCall(called_fn, args, "call");
    }
  } else {
    std::vector<llvm::Type*> arg_types;
    std::vector<Value*> args;
    // std::cerr << "function " << node->id << "\n";
    for (CalloutArg arg : node->callout_args) {
      // std::cerr << "arg is ";
      if (arg.index() == 0) {
        // std::cerr << "int32 or int32*\n";
        Value *arg_value; std::get<0>(arg)->accept(this); arg_value = ret;
        if (!arg_value) {
          AnnulReturnWithError("arg value was null!\n");
          return;
        }
        if (arg_value->getType()->isPointerTy()) {
          auto type = llvm::Type::getInt32PtrTy(context);
          arg_types.push_back(type);
          args.push_back(arg_value);
        } else {
          auto type = llvm::Type::getInt32Ty(context);
          arg_types.push_back(type);
          args.push_back(builder.CreateIntCast(arg_value, type, true));
        }
      } else {
        // std::cerr << "string (i8*)\n";
        Value *arg_value = builder.CreateGlobalStringPtr(
          StringRef(std::get<1>(arg)));
        arg_types.push_back(llvm::Type::getInt8PtrTy(context));
        args.push_back(arg_value);
      }
    }
    FunctionType *prototype = FunctionType::get(
      llvm::Type::getInt32Ty(context),
      arg_types,
      false
    );
    ret = GetCallout(node->id, prototype, std::move(args));
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
    auto type = TypeToLLVMType(v.type);
    AllocaInst *alloca = CreateEntryBlockAlloca(CurrentFunction(),
      type, v.id);
    AddScopedVar(v.id, {alloca, type}, shadow_list);
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

  CurrentFunction()->getBasicBlockList().push_back(merge_bb);
  builder.SetInsertPoint(merge_bb);
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

  BasicBlock *old_for_internal_bb = for_internal_bb;
  BasicBlock *old_for_after_bb = for_after_bb;
  BasicBlock *for_bb = BasicBlock::Create(context, "for_" + node->id, CurrentFunction());
  for_internal_bb = BasicBlock::Create(context, "for_internal_" + node->id);
  for_after_bb = BasicBlock::Create(context, "for_after_" + node->id);

  VarTable shadow_list;
  AllocaInst* i_alloca = CreateEntryBlockAlloca(CurrentFunction(), llvm::Type::getInt32Ty(context), node->id);
  AddScopedVar(node->id, {i_alloca, llvm::Type::getInt32Ty(context)}, shadow_list);
  builder.CreateStore(start_value, i_alloca);
  
  Value *brcond = builder.CreateICmpSLT(start_value, end_value);
  builder.CreateCondBr(brcond, for_bb, for_after_bb);
  builder.SetInsertPoint(for_bb);
  
  node->body->accept(this);

  if (!CurrentBlockDone()) {
    builder.CreateBr(for_internal_bb);
  }
  CurrentFunction()->getBasicBlockList().push_back(for_internal_bb);
  builder.SetInsertPoint(for_internal_bb);
  Value* i_val = builder.CreateAdd(builder.CreateLoad(i_alloca),
                                  GetConstIntN(32, 1));
  builder.CreateStore(i_val, i_alloca);
  builder.CreateCondBr(builder.CreateICmpSLT(i_val, end_value),
                    for_bb, for_after_bb);

  CurrentFunction()->getBasicBlockList().push_back(for_after_bb);
  builder.SetInsertPoint(for_after_bb);

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
    prototype, node->id == "main" ? Function::ExternalLinkage : Function::PrivateLinkage, node->id, module.get());
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
    auto type = prototype->getParamType(idx);
    AllocaInst *alloca = CreateEntryBlockAlloca(fn, type, arg.getName());
    builder.CreateStore(&arg, alloca);
    AddScopedVar(node->params[idx].id, {alloca, type}, shadow_list);
    idx++;
  }

  node->body->accept(this);
  // needed because every basic block *must* have a terminator
  // the return statement has no semantic effect.
  if (!CurrentBlockDone()) {
    if (node->IsVoid()) {
      builder.CreateRetVoid();
    } else {
      GenErrorAndExitInst("\nruntime error: reached end of function with non-void return.\n");
    }
  }
  verifyFunction(*fn, &errs());
  // fn->print(errs());
  // ret = fn;

  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(RootNode* node) {
  VarTable shadow_list;
  for (Field f : node->field_decls) {
    int length = 1;
    if (f.length != -1) {
      length = f.length;
      array_lengths[f.id] = f.length;
    }
    llvm::Type *type = TypeToLLVMType(f.type);
    GlobalVariable *ptr;
    if (length == 1) {
      ptr = (GlobalVariable*) module->getOrInsertGlobal(f.id, type);
      ptr->setInitializer(TypeToDefaultValue(f.type));
    } else {
      type = ArrayType::get(type, length);
      ptr = (GlobalVariable*) module->getOrInsertGlobal(f.id, type);
      ptr->setInitializer(ConstantArray::get((llvm::ArrayType*)type,
        ArrayRef(std::vector<Constant*>(length, TypeToDefaultValue(f.type)))));
    }
    AddScopedVar(f.id, {ptr, type}, shadow_list);
  }

  for (MethodNode *method : node->method_decls) {
    method->accept(this);
  }
  module->print(outs(), nullptr);

  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(UnvisitableNode* node) { }
void CodeGenVisitor::visit(TypeNode* node) { }
