#include "StatementNode.h"

#include <vector>

class enum Type {
  T_INT, T_BOOL, T_STRING;
};
struct Field {
  Type type;
  string id;
  int length;
};
struct Var {
  Type type;
  string id;
};

struct BlockNode {
  std::vector<Var> var_decls;
  std::vector<StatementNode> statements;
};

struct Method {
  Type return_type;
  string id;
  int length;
  ASTBlockNode *body;
};

struct RootNode {
public:
  std::vector<Field> field_decls;
  std::vector<Method> method_decls;
};

class enum StatementType {
  ASSIGN, METHOD_CALL, CALLOUT, IF, FOR, RETURN, BREAK, CONTINUE, BLOCK;
}
class enum AssignOp {
  ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN;
}
struct CalloutArg {
  ExprNode *maybe_expr;
  string maybe_string;
};

class StatementNode {
public:
  StatementType type;
  StatementNode(StatementType type);
};
class AssignNode : public StatementNode {
  string id;
  ExprNode *index;
  AssignOp op;q
  ExprNode *value;
};
class MethodCallNode : public StatementNode {
  string id;
  std::vector<ExprNode*> params;
};
class CalloutNode : public StatementNode {
  string id;
  std::vector<CalloutArg> params;
};
class IfNode : public StatementNode {
  ExprNode *cond;
  BlockNode *then, *otherwise;
};
class ForNode : public StatementNode {
  ExprNode *start, *end;
  BlockNode *body;
};
class BlockStatementNode : public StatementNode {
  BlockNode *body;
};
