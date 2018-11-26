#include "AstNode.h"

AstNode::~AstNode() { }

std::string TypeToString(Type t) {
  if (t == Type::INT_TYPE) {
    return "INT";
  } else if (t == Type::BOOLEAN_TYPE) {
    return "BOOL";
  } else if (t == Type::CHAR_TYPE) {
    return "CHAR";
  } else if (t == Type::STRING_TYPE) {
    return "STRING";
  } else if (t == Type::VOID_TYPE) {
    return "VOID";
  } else if (t == Type::ERROR) {
    return "ERROR";
  } 
}
