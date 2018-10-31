#include "driver.h"
#include <iostream>

void drive(AstNode *ast) {
  RootNode *root = (RootNode*)ast;
  std::cout << (root->id) << '\n';
}
