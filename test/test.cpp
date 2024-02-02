#include "dag.h"
#include <gtest/gtest.h>

#define ANCESTOR_AMOUNT 2
struct TestNode {
  DAGNode(node, ANCESTOR_AMOUNT);
};

TEST(init_node, normal) {
  struct TestNode node;
  struct DAGop op;
  DAGNodeInit(node.node, &op);
  EXPECT_EQ(node.node.ancestor_amount, ANCESTOR_AMOUNT);
  EXPECT_EQ(node.node.op, &op);
}
