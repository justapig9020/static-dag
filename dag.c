#include "dag.h"
#include <stdio.h>

const static struct DAGop default_dag_op;

static void add_child(struct DAGNode *self, struct DAGNode *child) {
    printf("add child\n");
    if (!self)
        return;
    if (!self->children) {
        self->children = child;
        return;
    }
    struct DAGNode *eldest_son = self->children;
    if (eldest_son->sibling != child)
        eldest_son->sibling = child;
}

static bool add_ancestor(struct DAGNode *self, struct DAGNode *ancestor) {
    printf("add ancestor\n");
    if (!self)
        return false;
    if (self->ancestor_count >= self->ancestor_amount) {
        return false;
    }
    add_child(ancestor, self);
    self->ancestor[self->ancestor_count++] = ancestor;
    return true;
}

void init_node(struct DAGNode *node, unsigned int ancestor_amount,
               void (*print_func)(struct DAGNode *node)) {
    node->ancestor_amount = ancestor_amount;
    node->ancestor_count = 0;
    node->op = &default_dag_op;
    node->print = print_func;
    node->children = NULL;
    node->sibling = NULL;
    for (int i = 0; i < ancestor_amount; i++) {
        node->ancestor[i] = NULL;
    }
}

void print_dag(struct DAGNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->print != NULL)
        node->print(node);
    printf("Siblings:\n");
    print_dag(node->sibling);
    printf("children:\n");
    print_dag(node->children);
}

const static struct DAGop default_dag_op = {
    .add_ancestor = add_ancestor,
    .release_ancestor = NULL,
};
