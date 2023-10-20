#include "dag.h"
#include <stdio.h>

#define DEBUG
#include "debug.h"

static void add_child(struct DAGNode *self, struct DAGNode *child) {
    if (!self)
        return;
    if (!self->children) {
        self->children = child;
        return;
    }
    struct DAGNode *eldest_son = self->children;
    if (eldest_son->sibling != child) {
        child->sibling = eldest_son->sibling;
        eldest_son->sibling = child;
    }
}

static bool add_ancestor(struct DAGNode *self, struct DAGNode *ancestor) {
    if (!self)
        return false;
    if (self->ancestor_count >= self->ancestor_amount) {
        return false;
    }
    add_child(ancestor, self);
    self->ancestor[self->ancestor_count++] = ancestor;
    return true;
}

bool has_ancestor(struct DAGNode *self) {
    if (!self)
        return false;
    return self->ancestor_count > 0;
}

bool is_ancestor_of(struct DAGNode *self, struct DAGNode *child) {
    if (!child || !self)
        return false;
    for (int i = 0; i < child->ancestor_count; i++) {
        if (child->ancestor[i] == self) {
            return true;
        }
    }
    return false;
}

static bool add_ancestors(struct DAGNode *self, va_list *ancestors) {
    struct DAGNode *ancestor;
    while ((ancestor = va_arg(*ancestors, struct DAGNode *)) != NULL) {
        bool success = add_ancestor(self, ancestor);
        if (!success)
            // TODO: clean up added ancestors
            return false;
    }
    return true;
}

void init_node(struct DAGNode *node, unsigned int ancestor_amount,
               struct DAGop *op, va_list *ancestors) {

    node->ancestor_amount = ancestor_amount;
    node->ancestor_count = 0;
    node->op = op;
    node->children = NULL;
    node->sibling = NULL;

    return add_ancestors(node, ancestors);
}

static void print_dag_inner(struct DAGNode *root) {
    if (!root)
        return;

    if (root->op->print != NULL)
        root->op->print(root);

    for_each_child(root, child) print_dag_inner(child);
    for_each_child(root, child) debug("%p --> %p;\n", root, child);
}

void print_dag(struct DAGNode *root) {
    if (!root)
        return;
    debug("```mermaid\n");
    debug("flowchart LR;\n");
    print_dag_inner(root);
    debug("```");
}

struct DAGNode *next_child(struct DAGNode *self,
                           struct DAGNode *current_child) {
    if (!self || !current_child)
        return NULL;
    while (current_child->sibling) {
        current_child = current_child->sibling;
        if (is_ancestor_of(self, current_child))
            return current_child;
    }
    return NULL;
}