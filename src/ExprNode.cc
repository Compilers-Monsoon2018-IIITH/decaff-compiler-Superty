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
: id(ReduceToString(o_id)) {
  ReduceToVector(o_args, &args);
}
MethodCallNode::MethodCallNode(StringLitNode *o_id, ListNode<CalloutArg>* o_callout_args)
: id(ReduceToString(o_id)) {
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
