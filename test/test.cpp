#include "dag.h"
#include <gtest/gtest.h>

#define ANCESTOR_AMOUNT 5

struct TestNode {
  bool freed;
  bool all_ancestors_freed;
  DAGNode(node, ANCESTOR_AMOUNT);
};

void free_test_node(struct DAGNode *self) {
  struct TestNode *node = container_of(self, struct TestNode, node);
  node->freed = true;
}

void set_all_ancestores_freed(struct DAGNode *self) {
  struct TestNode *node = container_of(self, struct TestNode, node);
  node->all_ancestors_freed = true;
}

struct DAGop default_op = {
    .free = free_test_node,
    .print = NULL,
    .on_all_anecestors_freed = set_all_ancestores_freed,
};

void init_test_node(struct TestNode *self) {
  self->freed = false;
  self->all_ancestors_freed = false;
  DAGNodeInit(self->node, &default_op);
}

bool is_test_node_freed(struct TestNode *self) { return self->freed; }

bool is_all_ancestors_freed_triggered(struct TestNode *self) {
  return self->all_ancestors_freed;
}

TEST(Init, Normal) {
  struct TestNode node;
  DAGNodeInit(node.node, &default_op);
  EXPECT_EQ(node.node.ancestor_amount, ANCESTOR_AMOUNT);
  EXPECT_EQ(node.node.op, &default_op);
}

TEST(Add, One) {
  struct TestNode ancestor;
  DAGNodeInit(ancestor.node, &default_op);
  struct TestNode child;
  DAGNodeInit(child.node, &default_op);
  bool result = add_ancestor(&child.node, &ancestor.node);
  EXPECT_EQ(result, true);
  EXPECT_EQ(ancestor_count(&child.node), 1);
  EXPECT_EQ(has_ancestor(&child.node), true);
  EXPECT_EQ(is_ancestor_of(&ancestor.node, &child.node), true);
}

TEST(Add, Multiple) {
  const unsigned int ancestor_amount = ANCESTOR_AMOUNT;
  struct TestNode ancestors[ancestor_amount];
  for (unsigned int i = 0; i < ancestor_amount; i++) {
    DAGNodeInit(ancestors[i].node, &default_op);
  }
  struct TestNode child;
  DAGNodeInit(child.node, &default_op);
  for (unsigned int i = 0; i < ancestor_amount; i++) {
    bool result = add_ancestor(&child.node, &ancestors[i].node);
    EXPECT_EQ(result, true);
  }
  EXPECT_EQ(ancestor_count(&child.node), ancestor_amount);
  EXPECT_EQ(has_ancestor(&child.node), true);
  for (unsigned int i = 0; i < ancestor_amount; i++) {
    EXPECT_EQ(is_ancestor_of(&ancestors[i].node, &child.node), true);
  }
}

TEST(Add, Twice) {
  struct TestNode ancestor;
  DAGNodeInit(ancestor.node, &default_op);
  struct TestNode child;
  DAGNodeInit(child.node, &default_op);
  bool result;
  result = add_ancestor(&child.node, &ancestor.node);
  EXPECT_EQ(result, true);
  result = add_ancestor(&child.node, &ancestor.node);
  EXPECT_EQ(result, true);

  EXPECT_EQ(ancestor_count(&child.node), 1);
  EXPECT_EQ(has_ancestor(&child.node), true);
  EXPECT_EQ(is_ancestor_of(&ancestor.node, &child.node), true);
}

TEST(Add, EmptyNode) {
  struct TestNode child;
  DAGNodeInit(child.node, &default_op);
  bool result = add_ancestor(&child.node, NULL);
  EXPECT_EQ(result, true);
  EXPECT_EQ(ancestor_count(&child.node), 0);
  EXPECT_EQ(has_ancestor(&child.node), false);
}

TEST(Add, EmptyChild) {
  struct TestNode ancestor;
  DAGNodeInit(ancestor.node, &default_op);
  bool result = add_ancestor(NULL, &ancestor.node);
  EXPECT_EQ(result, false);
}

TEST(Remove, NoAncestor) {
  struct TestNode child;
  init_test_node(&child);
  EXPECT_EQ(is_test_node_freed(&child), false);
  bool result = remove_node(&child.node);
  EXPECT_EQ(result, true);
  EXPECT_EQ(is_test_node_freed(&child), true);
}

TEST(Remove, OneAncestor) {
  struct TestNode ancestor;
  init_test_node(&ancestor);
  struct TestNode child;
  init_test_node(&child);
  add_ancestor(&child.node, &ancestor.node);
  EXPECT_EQ(is_test_node_freed(&child), false);
  bool result = remove_node(&child.node);
  EXPECT_EQ(result, false);
  EXPECT_EQ(is_test_node_freed(&child), false);
}

TEST(Remove, MultipleAncestors) {
  const unsigned int ancestor_amount = ANCESTOR_AMOUNT;
  struct TestNode ancestors[ancestor_amount];
  for (unsigned int i = 0; i < ancestor_amount; i++) {
    init_test_node(&ancestors[i]);
  }
  struct TestNode child;
  init_test_node(&child);
  for (unsigned int i = 0; i < ancestor_amount; i++) {
    add_ancestor(&child.node, &ancestors[i].node);
  }
  EXPECT_EQ(is_test_node_freed(&child), false);
  bool result = remove_node(&child.node);
  EXPECT_EQ(result, false);
  EXPECT_EQ(is_test_node_freed(&child), false);

  for (unsigned int i = 0; i < ancestor_amount; i++) {
    remove_node(&ancestors[i].node);
  }

  result = remove_node(&child.node);
  EXPECT_EQ(ancestor_count(&child.node), 0);
  EXPECT_EQ(result, true);
  EXPECT_EQ(is_test_node_freed(&child), true);
}

TEST(Remove, AllAncestorsFreedCallback) {
  struct TestNode ancestor;
  init_test_node(&ancestor);
  struct TestNode child;
  init_test_node(&child);
  add_ancestor(&child.node, &ancestor.node);
  EXPECT_EQ(is_all_ancestors_freed_triggered(&child), false);
  remove_node(&ancestor.node);
  EXPECT_EQ(is_all_ancestors_freed_triggered(&child), true);
}
