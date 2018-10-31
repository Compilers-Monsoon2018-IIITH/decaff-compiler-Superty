#include "driver.h"
#include <iostream>
#include "TypeCheckVisitor.h"

void drive(AstNode *ast) {
  RootNode *root = (RootNode*)ast;
  std::cout << (root->id) << '\n';
  TypeCheckVisitor v;
  root->accept(&v);
  std::cout << "Type check " << (v.success ? 
    "succeeded." : "failed!") << '\n';
}
