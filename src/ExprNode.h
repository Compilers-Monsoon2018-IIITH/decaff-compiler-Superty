#include <string>
#include <vector>

enum class ExprType {
  LOCATION, CALL, INT_LIT, CHAR_LIT, STRING_LIT, BOOL_LIT, BINOP, UNOP
};
  class ExprNode {
public:
  ExprType type;
protected:
  ExprNode(ExprType o_type);
};

struct CalloutArg {
  ExprNode *maybe_expr;
  std::string maybe_string;
};
struct Location {
  std::string id;
  ExprNode *index;
};
enum class BinOp {
OR, AND, EQ, NE, LT, LE, GE, GT, PLUS, MINUS, MULT, DIV, MOD };
enum class UnOp {
  NEG, NOT
};

class LocationNode : public ExprNode {
public:
  Location location;
  LocationNode();
};
class MethodCallNode : public ExprNode {
public:
  std::string id;
  std::vector<ExprNode*> args;
  std::vector<CalloutArg> callout_args;
  MethodCallNode();
};
class IntLitNode : public ExprNode {
public:
  int value;
  IntLitNode();
};
class CharLitNode : public ExprNode {
public:
  char value;
  CharLitNode();
};
class StringLitNode : public ExprNode {
public:
  std::string value;
  StringLitNode();
};
class BoolLitNode : public ExprNode {
public:
  bool value;
  BoolLitNode();
};
class BinopNode : public ExprNode {
public:
  BinOp op;
  ExprNode *left, *right;
  BinopNode();
};
class UnopNode : public ExprNode {
public:
  UnOp op;
  ExprNode *expr;
  UnopNode();
};
