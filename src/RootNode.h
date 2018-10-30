#include "BlockNode.h"

#include <vector>

class enum Type {
  T_INT, T_BOOL, T_STRING;
};
struct Field {
  Type type;
  string id;
  int length;
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
