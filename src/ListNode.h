#pragma once

#include "AstNode.h"
#include "AstVisitor.h"
#include <vector>

template <class T>
class ListNode : public UnvisitableNode {
public:
  T value;
  ListNode<T>* next;
  ListNode(T o_value, ListNode<T>* o_next)
  : value(o_value), next(o_next) { }
};

template <class T>
void ReduceToVector(ListNode<T>* node, std::vector<T> *output) {
  if (node == nullptr) {
    return;
  }
  output->push_back(node->value);
  ListNode<T>* next = node->next;
  delete node;
  ReduceToVector(next, output);
}
