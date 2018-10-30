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
  AssignOp op;
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