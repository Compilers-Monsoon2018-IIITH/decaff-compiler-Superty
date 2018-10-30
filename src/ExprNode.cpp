#include "ExprNode.h"

ExprNode::ExprNode(ExprType o_type)
: type(o_type) {}
LocationNode::LocationNode()
: ExprNode(ExprType::LOCATION) { }
MethodCallNode::MethodCallNode()
: ExprNode(ExprType::CALL) { }
IntLitNode::IntLitNode()
: ExprNode(ExprType::INT_LIT) { }
CharLitNode::CharLitNode()
: ExprNode(ExprType::CHAR_LIT) { }
StringLitNode::StringLitNode()
: ExprNode(ExprType::STRING_LIT) { }
BoolLitNode::BoolLitNode()
: ExprNode(ExprType::BOOL_LIT) { }
BinopNode::BinopNode()
: ExprNode(ExprType::BINOP) { }
UnopNode::UnopNode()
: ExprNode(ExprType::UNOP) { }