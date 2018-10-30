#include "ExprNode.h"

class enum StatementType {
  ASSIGN, CALL, IF, FOR, RETURN, BREAK, CONTINUE, BLOCK;
}
class enum AssignOp {
  ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN;
}
struct Var {
  Type type;
  string id;
}

class StatementNode {
public:
  StatementType type;
  StatementNode(StatementType type);
};
class AssignNode : public StatementNode {
public:
  Location location;
  AssignOp op;
  ExprNode *value;
};
class StatementCallNode : public StatementNode {
public:
  MethodCallNode *node;
}
class IfNode : public StatementNode {
public:
  ExprNode *cond;
  BlockNode *then, *otherwise;
};
class ForNode : public StatementNode {
public:
  ExprNode *start, *end;
  BlockNode *body;
};
class ReturnNode : public StatementNode {
public:
  ExprNode *value;
};
class BlockNode : public StatementNode {
public:
  std::vector<Var> var_decls;
  std::vector<StatementNode*> statements;
};
