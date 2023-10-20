#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr)-offsetof(type, member)))

#define DAGNode(name, ancestor_count)                                          \
    struct DAGNode name;                                                       \
    struct DAGNode *name##_ancestor[ancestor_count];

#define DAGNodeInit(name, op)                                                  \
    init_node(&(name), sizeof(name##_ancestor) / sizeof(name##_ancestor[0]),   \
              (op));

#define for_each_child(self, child)                                            \
    for (struct DAGNode *child = self->children; child;                        \
         child = next_child(self, child))

struct DAGNode {
    struct DAGop *op;
    struct DAGNode *children;
    struct DAGNode *sibling;
    unsigned int ancestor_amount;
    unsigned int ancestor_count;
    struct DAGNode *ancestor[0];
};

struct DAGop {
    void (*print)(struct DAGNode *node);
};

void init_node(struct DAGNode *node, unsigned int ancestor_amount,
               struct DAGop *op);

void print_dag(struct DAGNode *root);

bool add_ancestor(struct DAGNode *self, struct DAGNode *ancestor);
bool has_ancestor(struct DAGNode *self);
bool is_ancestor_of(struct DAGNode *self, struct DAGNode *child);
struct DAGNode *next_child(struct DAGNode *self, struct DAGNode *current_child);