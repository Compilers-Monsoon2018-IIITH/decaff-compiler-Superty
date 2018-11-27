#include "ExprNode.h"

CharLitNode::CharLitNode(char o_value)
: value(o_value) { }
StringLitNode::StringLitNode(std::string o_value)
: value(o_value) { }
IntLitNode::IntLitNode(int o_value)
: value(o_value) { }
BoolLitNode::BoolLitNode(bool o_value)
: value(o_value) { }
LocationNode::LocationNode(StringLitNode *o_id, AstNode* o_index)
: id(ReduceToString(o_id)), index(o_index) { }
MethodCallNode::MethodCallNode(StringLitNode *o_id, ListNode<AstNode*>* o_args)
: id(ReduceToString(o_id))
, is_callout(false) {
  ReduceToVector(o_args, &args);
}
MethodCallNode::MethodCallNode(StringLitNode *o_id, ListNode<CalloutArg>* o_callout_args)
: id(ReduceToString(o_id))
, is_callout(true) {
  ReduceToVector(o_callout_args, &callout_args);
}
BinopNode::BinopNode(AstNode* o_left, Op o_op, AstNode *o_right)
: left(o_left), op(o_op), right(o_right) { }
UnopNode::UnopNode(Op o_op, AstNode* o_expr)
: op(o_op), expr(o_expr) { }

std::string ReduceToString(StringLitNode* node) {
  std::string result = std::move(node->value);
  delete node;
  return result;
}
int ReduceToInt(IntLitNode* node) {
  int result = node->value;
  delete node;
  return result;
}

std::string OpToString(Op op) {
  switch (op) {
    case Op::PLUS:
      return "+";
    case Op::MINUS:
      return "-";
    case Op::MULT:
      return "*";
    case Op::DIV:
      return "/";
    case Op::MOD:
      return "%";
    case Op::EQ:
      return "==";
    case Op::NE:
      return "!=";
    case Op::LT:
      return "<";
    case Op::LE:
      return "<=";
    case Op::GT:
      return ">";
    case Op::GE:
      return ">=";
    case Op::OR:
      return "||";
    case Op::AND:
      return "&&";
  }
}