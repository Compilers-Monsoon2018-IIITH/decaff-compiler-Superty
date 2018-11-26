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

llvm::Value* CodeGenVisitor::TypeToDefaultValue(Type type) {
  if (type != Type::INT_TYPE && type != Type::BOOLEAN_TYPE) {
    std::cerr << "WARNING: no default value exists for non-int non-bool types.\n";
  }
  return llvm::ConstantInt::get(context,
    llvm::APInt(type == Type::INT_TYPE ? 32 : 1, 0, true));
}

using namespace llvm;

CodeGenVisitor::CodeGenVisitor()
: builder(context)
, module(std::move(llvm::make_unique<Module>("module", context))) { }

void CodeGenVisitor::AnnulReturnWithError(const std::string& error) {
  std::cerr << error << "Returning nullptr.\n";
  ret = nullptr;
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

void CodeGenVisitor::AddScopedVar(const std::string& name, AllocaInst* alloca, VarTable& shadow_list) {
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

AllocaInst *CreateEntryBlockAlloca(Function *fn, llvm::Type* type, const std::string &id) {
  IRBuilder<> temp_builder(&fn->getEntryBlock(),
                 fn->getEntryBlock().begin());
  return temp_builder.CreateAlloca(type, 0,
                           id.c_str());
}

void CodeGenVisitor::visit(IntLitNode* node) {
  ret = ConstantInt::get(context, APInt(32, node->value, true));
}
void CodeGenVisitor::visit(CharLitNode* node) {
  ret = ConstantInt::get(context, APInt(8, node->value, false));
}
void CodeGenVisitor::visit(StringLitNode* node) {
  // ret = builder.CreateGlobalStringPtr(StringRef(node->value));
}
void CodeGenVisitor::visit(BoolLitNode* node) {
  ret = ConstantInt::get(context, APInt(1, node->value, true));
}
void CodeGenVisitor::visit(LocationNode* node) {
  if (node->index == nullptr) {
    ret = builder.CreateLoad(vars[node->id], node->id);
  } else {
    // TODO: HANDLE ARRAYS
  }
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
  Function* fn = builder.GetInsertBlock()->getParent();
  VarTable shadow_list;
  for (Var v : node->var_decls) {
    AllocaInst *alloca = CreateEntryBlockAlloca(fn,
      TypeToLLVMType(v.type), v.id);
    AddScopedVar(v.id, alloca, shadow_list);
    builder.CreateStore(TypeToDefaultValue(v.type), alloca);
  }
  for (auto statement : node->statements) {
    statement->accept(this);
  }
  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(AssignNode* node) {
  Value *value; node->value->accept(this); value = ret;
  auto alloca = vars[node->location->id];
  // TODO: HANDLE ARRAYS
  if (node->location->index == nullptr) {
    if (node->op != AssignOp::ASSIGN) {
      Value *cur_value = builder.CreateLoad(alloca, node->location->id);
      if (node->op == AssignOp::PLUS_ASSIGN) {
        value = builder.CreateAdd(cur_value, value, "plus_assign");
      } else {
        value = builder.CreateSub(cur_value, value, "plus_assign");
      }
    }
    builder.CreateStore(value, alloca);
  }
}
void CodeGenVisitor::visit(IfNode* node) {
  
}
void CodeGenVisitor::visit(ForNode* node) {
  
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
  verifyFunction(*fn, &errs());
  // fn->print(errs());
  // ret = fn;

  RestoreShadowedVars(shadow_list);
}
void CodeGenVisitor::visit(RootNode* node) {
  // TODO: Handle field decls

  for (MethodNode *method : node->method_decls) {
    method->accept(this);
  }
  module->print(outs(), nullptr);
}
void CodeGenVisitor::visit(UnvisitableNode* node) { }
void CodeGenVisitor::visit(TypeNode* node) { }
