#include "RootNode.h"

MethodNode::MethodNode(Type o_return_type, std::string o_id, VarList *var_list, BlockNode *o_body)
: return_type(o_return_type), id(o_id), body(o_body) {
  ReduceToVector(var_list, &params);
}

bool MethodNode::IsVoid() {
  return return_type == Type::VOID_TYPE; 
}

MethodNode* ConstructMethodNode(AstNode* ret, AstNode* id, AstNode* var_list, AstNode* body) {
  return new MethodNode(
    ReduceToType((TypeNode*)ret),
    ReduceToString((StringLitNode*)id),
    (VarList*)var_list,
    (BlockNode*)body
    );
}

void RootNode::AppendFields(TypeNode* type, FieldList* ids) {
  Type t = ReduceToType(type);
  std::vector<Field> fields_vector;
  ReduceToVector(ids, &fields_vector);
  for (Field field : fields_vector) {
    field.type = t;
    field_decls.push_back(field);
  }
}
