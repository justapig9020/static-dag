#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr)-offsetof(type, member)))

#define DAGNode(name, ancestor_count)                                          \
    struct DAGNode name;                                                       \
    struct DAGNode *name##_ancestor[ancestor_count];

#define DAGNodeInit(name, print_func)                                          \
    init_node(&name, sizeof(name##_ancestor) / sizeof(name##_ancestor[0]),     \
              print_func);

struct DAGNode {
    const struct DAGop *op;
    void (*print)(struct DAGNode *node);
    struct DAGNode *children;
    struct DAGNode *sibling;
    unsigned int ancestor_amount;
    unsigned int ancestor_count;
    struct DAGNode *ancestor[0];
};

struct DAGop {
    bool (*add_ancestor)(struct DAGNode *self, struct DAGNode *ancestor);
    bool (*release_ancestor)(struct DAGNode *self, struct DAGNode *ancestor);
};

void init_node(struct DAGNode *node, unsigned int ancestor_amount,
               void (*print_func)(struct DAGNode *node));

void print_dag(struct DAGNode *node);