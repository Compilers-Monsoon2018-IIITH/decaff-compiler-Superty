#include "BlockNode.h"

TypeNode::TypeNode(Type o_type)
: type(o_type) { }
Type ReduceToType(TypeNode *node) {
  Type type = node->type;
  delete node;
  return type;
}

Var ReduceToVar(AstNode *type, AstNode *id) {
  return {
    ReduceToType((TypeNode*)type),
    ReduceToString((StringLitNode*)id)
  };
}

void BlockNode::AppendVars(TypeNode* type, IdList* ids) {
  Type t = ReduceToType(type);
  std::vector<StringLitNode*> nodes_vector;
  ReduceToVector(ids, &nodes_vector);
  for (StringLitNode* node : nodes_vector) {
    var_decls.push_back({t, ReduceToString(node)});
  }
}
void BlockNode::SetStatements(ListNode<AstNode*> *o_statements) {
  ReduceToVector(o_statements, &statements);
}

AssignNode::AssignNode(LocationNode *o_location, AssignOp o_op, AstNode *o_value)
: location(o_location), op(o_op), value(o_value) { }

IfNode::IfNode(AstNode *o_cond, BlockNode *o_then, BlockNode *o_otherwise)
: cond(o_cond), then(o_then), otherwise(o_otherwise) { }

ForNode::ForNode(std::string o_id, AstNode *o_start, AstNode *o_end, BlockNode *o_body)
: id(o_id), start(o_start), end(o_end), body(o_body) { }

ReturnNode::ReturnNode(AstNode *o_value)
: value(o_value) { }

LoopControlNode::LoopControlNode(LoopControlType o_type)
: type(o_type) { }
