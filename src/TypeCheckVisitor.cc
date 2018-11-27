#include "TypeCheckVisitor.h"
#include <iostream>
#include <unistd.h>

const bool VERBOSE_DEBUG_OUTPUT = false;

TypeCheckVisitor::TypeCheckVisitor()
: success(true)
, for_depth(0)
, enclosing_return_type(Type::VOID_TYPE) { }

bool TypeCheckVisitor::Ensure(bool predicate) {
  if (!predicate) {
    success = false;
  }
  return predicate;
}

void TypeCheckVisitor::ForceSame(Type& t, Type u) {
  if (t == Type::UNKNOWN) {
    t = u;
  } else {
    Ensure(t == u);
  }
}

void DumpVar(Var v) {
  if (!VERBOSE_DEBUG_OUTPUT) {
    return;
  }
  std:: cerr << TypeToString(v.type) << ' ' << v.id << "; ";
}

void TypeCheckVisitor::DumpVars() {
  if (!VERBOSE_DEBUG_OUTPUT) {
    return;
  }
  std::cerr << "~~~~VARS~~~~\n";
  for (auto pr : vars) {
    std::cerr << TypeToString(pr.second.first) << (pr.second.second ? "[]" : "") << ' ' << pr.first << '\n';

  }
}

void TypeCheckVisitor::AddScopedVar(Var var, VarTable& shadow_list) {
  if (vars.count(var.id) > 0) {
    // two params with same name
    if (Ensure(shadow_list.count(var.id) == 0)) {
      shadow_list[var.id] = vars[var.id]; // shadowing
    }
  } else {
    shadow_list[var.id] = {Type::VOID_TYPE, false};
    // store a placeholder so we know to remove the id later
  }
  vars[var.id] = {var.type, false};
}
void TypeCheckVisitor::RestoreShadowedVars(const VarTable& shadow_list) {
  for (auto pr : shadow_list) {
    if (VERBOSE_DEBUG_OUTPUT) std::cerr << "Restoring " << pr.first << ' ' << TypeToString(pr.second.first) << '\n';
    if (pr.second.first == Type::VOID_TYPE) {
      vars.erase(pr.first);
    } else {
      vars[pr.first] = pr.second; // unshadow old variables
    }
  }
}

void TypeCheckVisitor::visit(IntLitNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======IntLitNode " << node->value << "\n";
  }
  node->t = Type::INT_TYPE;
}
void TypeCheckVisitor::visit(CharLitNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======CharLitNode " << node->value << "\n";
  }
  node->t = Type::CHAR_TYPE;
}
void TypeCheckVisitor::visit(StringLitNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======StringLitNode " << node->value << "\n";
  }
  node->t = Type::STRING_TYPE;
}
void TypeCheckVisitor::visit(BoolLitNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======BoolLitNode " << node->value << "\n";
  }
  node->t = Type::BOOLEAN_TYPE;
}
void TypeCheckVisitor::visit(LocationNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======LocationNode " << node->id << "\n";
  }
  if (node->index != nullptr) {
    node->index->accept(this);
  }

  if (Ensure(vars.count(node->id) > 0)) {
    if ((node->index != nullptr) == (vars[node->id].second)) {
      if (node->index == nullptr || node->index->t == Type::INT_TYPE) {
        node->t = vars[node->id].first;
        return;
      }
    }
  }
  std::cerr << "ERROR LOCATION: " << node->id << "; " << (node->index == nullptr ? "no index" : TypeToString(node->index->t)) << '\n';
  DumpVars();
  node->t = Type::ERROR;
  Ensure(false);
}
void TypeCheckVisitor::visit(MethodCallNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======MethodCallNode " << node->id << "\n";
  }
  if (node->args.empty()) {
    for (CalloutArg arg : node->callout_args) {
      if (arg.index() == 0) {
        std::get<0>(arg)->accept(this);
      }
    }
    node->t = Type::INT_TYPE;
  } else {
    if (Ensure(methods.count(node->id) > 0 && vars.count(node->id) == 0)) {
      Ensure(node->args.size() == methods[node->id].size() - 1);
      auto& params = methods[node->id];
      for (int i = 0; i < node->args.size(); i++) {
        node->args[i]->accept(this);
        ForceSame(node->args[i]->t, params[i + 1]);
      }
      node->t = params[0];
    }
  }
}
void TypeCheckVisitor::visit(BinopNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======BinopNode " << OpToString(node->op) << "\n";
  }
  node->left->accept(this);
  node->right->accept(this);
  if (node->left->t == Type::UNKNOWN) {
    node->left->t = node->right->t;
  } else if (node->right->t == Type::UNKNOWN) {
    node->right->t = node->left->t;
  }
  Ensure(node->left->t == node->right->t);
  switch (node->op) {
    case Op::PLUS:
    case Op::MINUS:
    case Op::MULT:
    case Op::DIV:
    case Op::MOD:
      ForceSame(node->left->t, Type::INT_TYPE);
      node->t = Type::INT_TYPE;
      break;
    case Op::EQ:
    case Op::NE:
    case Op::LT:
    case Op::LE:
    case Op::GT:
    case Op::GE:
      node->t = Type::BOOLEAN_TYPE;
      break;
    case Op::OR:
    case Op::AND:
      ForceSame(node->left->t, Type::BOOLEAN_TYPE);
      node->t = Type::BOOLEAN_TYPE;
      break;
  }
}
void TypeCheckVisitor::visit(UnopNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======UnopNode " << OpToString(node->op) << "\n";
  }
  node->expr->accept(this);
  if (node->op == Op::MINUS) {
    Ensure(node->expr->t == Type::INT_TYPE);
  }
  if (node->op == Op::NOT) {
    Ensure(node->expr->t == Type::BOOLEAN_TYPE);
  }
}
void TypeCheckVisitor::visit(BlockNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======BlockNode\n";
  }
  VarTable shadow_list;
  for (Var var : node->var_decls) {
    AddScopedVar(var, shadow_list);
  }

  DumpVars();

  for (AstNode *statement_node : node->statements) {
    statement_node->accept(this);
  }

  RestoreShadowedVars(shadow_list);
}
void TypeCheckVisitor::visit(AssignNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======AssignNode " << "\n";
  }
  node->location->accept(this);
  node->value->accept(this);

  ForceSame(node->value->t, node->location->t);
  Ensure(node->op == AssignOp::ASSIGN || (
    node->value->t == Type::INT_TYPE));
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(IfNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======IfNode\n";
  }
  node->cond->accept(this);
  node->then->accept(this);
  if (node->otherwise != nullptr) {
    node->otherwise->accept(this);
  }

  Ensure(node->cond->t == Type::BOOLEAN_TYPE);
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(ForNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======ForNode " << node->id << " \n";
  }
  node->start->accept(this);
  node->end->accept(this);
  Ensure(node->start->t == Type::INT_TYPE &&
          node->end->t == Type::INT_TYPE);

  VarTable shadow_list;
  AddScopedVar(Var{Type::INT_TYPE, node->id}, shadow_list);

  for_depth++;
  node->body->accept(this);
  for_depth--;
  node->t = Type::VOID_TYPE;

  RestoreShadowedVars(shadow_list);
}
void TypeCheckVisitor::visit(ReturnNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======ReturnNode\n";
  }

  if (node->value != nullptr) {
    node->value->accept(this);
    Ensure(enclosing_return_type == node->value->t);
  } else {
    Ensure(enclosing_return_type == Type::VOID_TYPE);
  }
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(LoopControlNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======LoopControlNode\n";
  }
  Ensure(for_depth > 0);
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(MethodNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======MethodNode " << (node->id) << "\n";
  }
  node->t = Type::VOID_TYPE;

  VarTable shadow_list;
  if (VERBOSE_DEBUG_OUTPUT) std::cerr << "node->params: ";
  for (Var var : node->params) {
    DumpVar(var);
    AddScopedVar(var, shadow_list);
  }
  // std::cerr << '\n';
  DumpVars();

  enclosing_return_type = node->return_type;
  node->body->accept(this);

  // std::cerr << "methodnode" << (node->id) << " almost done.\n";

  RestoreShadowedVars(shadow_list);

  // std::cerr << "methodnode" << (node->id) << " done,\n";
}

void TypeCheckVisitor::visit(RootNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======RootNode\n";
  }
  node->t = Type::VOID_TYPE;

  for (Field f : node->field_decls) {
    if (Ensure(vars.count(f.id) == 0)) {
      if (f.length != -1) {
        Ensure(f.length > 0);
      }
      vars[f.id] = {f.type, f.length != -1};
      if (VERBOSE_DEBUG_OUTPUT) {
        std::cerr << f.id << ", ";
        std::cerr << TypeToString(f.type) << ' ';
        std::cerr << f.length << '\n';
      }
    }
  }

  bool main_found = false;
  for (MethodNode *method_node : node->method_decls) {
    // add method to methods
    Ensure(methods.count(method_node->id) == 0);
    methods[method_node->id] = {method_node->return_type};
    if (method_node->id == "main" && method_node->params.empty()) {
      main_found = true;
    }
    for (Var var : method_node->params) {
      methods[method_node->id].push_back(var.type);
    }

    // std::cerr << "method id: " << method_node->id << '\n'; 
    method_node->accept(this);
  }
  // if (!Ensure(main_found)) {
  //   std::cerr << "Main not found!\n"
  // }
}

void TypeCheckVisitor::visit(UnvisitableNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======UnvisitableNode\n";
  }

}
void TypeCheckVisitor::visit(TypeNode* node) {
  if (VERBOSE_DEBUG_OUTPUT) {
    /*trace*/std::cerr << "=======TypeNode\n";
  }

}
