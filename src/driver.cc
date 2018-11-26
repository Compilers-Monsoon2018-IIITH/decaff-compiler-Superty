#include "driver.h"
#include <iostream>
#include "TypeCheckVisitor.h"
#include "CodeGenVisitor.h"

void drive(AstNode *ast) {
  RootNode *root = (RootNode*)ast;
  TypeCheckVisitor v;
  root->accept(&v);
  if (!v.success) {
    std::cerr << "Type check failed!\n";
    return;
  }
  CodeGenVisitor code_generator;
  root->accept(&code_generator);
}
