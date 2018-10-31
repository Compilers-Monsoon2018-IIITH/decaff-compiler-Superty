#include "TypeCheckVisitor.h"

TypeCheckVisitor::TypeCheckVisitor()
: success(true) { }

void TypeCheckVisitor::visit(IntLitNode* node) {
  node->t = Type::INT_TYPE;
}
void TypeCheckVisitor::visit(CharLitNode* node) {
  node->t = Type::CHAR_TYPE;
}
void TypeCheckVisitor::visit(StringLitNode* node) {
  node->t = Type::STRING_TYPE;
}
void TypeCheckVisitor::visit(BoolLitNode* node) {
  node->t = Type::BOOLEAN_TYPE;
}
void TypeCheckVisitor::visit(LocationNode* node) {
  if (node->index != nullptr) {
    node->index->accept(this);
  }

  if (vars.count(node->id) > 0) {
    if ((node->index != nullptr) == (vars[node->id].second)) {
      if (node->index->t == Type::INT_TYPE) {
        node->t = vars[node->id].first;
        return;
      }
    }
  }
  node->t = Type::ERROR;
  success = false;
}
void TypeCheckVisitor::visit(MethodCallNode* node) {
  if (node->args.empty()) {
    for (CalloutArg arg : node->callout_args) {
      if (arg.index() == 0) {
        std::get<0>(arg)->accept(this);
      }
    }

    // TODO
  } else {
    for (AstNode *arg : node->args) {
      arg->accept(this);
    }
  }
}
void TypeCheckVisitor::visit(BinopNode* node) {
  node->left->accept(this);
  node->right->accept(this);
  switch (node->op) {
    case Op::PLUS:
    case Op::MINUS:
    case Op::MULT:
    case Op::DIV:
    case Op::MOD:
      if (node->left->t != Type::INT_TYPE || node->right->t != Type::INT_TYPE) {
        success = false;
      }
      break;
    case Op::EQ:
    case Op::NE:
    case Op::LT:
    case Op::LE:
    case Op::GT:
    case Op::GE:
      if (node->left->t != node->right->t) {
        success = false;
      }
      break;
    case Op::OR:
    case Op::AND:
      if (node->left->t != Type::BOOLEAN_TYPE || node->right->t != Type::BOOLEAN_TYPE) {
        success = false;
      }
      break;
  }
}
void TypeCheckVisitor::visit(UnopNode* node) {
  node->expr->accept(this);
  if (node->op == Op::MINUS) {
    if (node->expr->t != Type::INT_TYPE) {
      success = false;
    }
  }
  if (node->op == Op::NOT) {
    if (node->expr->t != Type::BOOLEAN_TYPE) {
      success = false;
    }
  }
}
void TypeCheckVisitor::visit(BlockNode* node) {
  // add vars to symbol; remove on exit.

  for (AstNode *statement_node : node->statements) {
    statement_node->accept(this);
  }
}
void TypeCheckVisitor::visit(AssignNode* node) {
  node->location->accept(this);
  node->value->accept(this);

  if (node->location->t != node->value->t) {
    success = false;
  }
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(IfNode* node) {
  node->cond->accept(this);
  node->then->accept(this);
  if (node->otherwise != nullptr) {
    node->otherwise->accept(this);
  }

  if (node->cond->t != Type::BOOLEAN_TYPE) {
    success = false;
  }
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(ForNode* node) {
  node->start->accept(this);
  node->end->accept(this);
  node->body->accept(this);
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(ReturnNode* node) {
  if (node->value != nullptr) {
    node->value->accept(this);
  }
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(LoopControlNode* node) {
  node->t = Type::VOID_TYPE;
}
void TypeCheckVisitor::visit(MethodNode* node) {
  node->t = Type::VOID_TYPE;

  node->body->accept(this);
}
void TypeCheckVisitor::visit(RootNode* node) {
  node->t = Type::VOID_TYPE;

  // add fields to symbol

  for (MethodNode *method_node : node->method_decls) {
    // add method to methods

    method_node->accept(this);
  }
}

void TypeCheckVisitor::visit(UnvisitableNode* node) { }
void TypeCheckVisitor::visit(TypeNode* node) { }
