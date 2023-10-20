#include "dag.h"
#include <stdio.h>

#define DEBUG
#include "debug.h"

static void add_child(struct DAGNode *self, struct DAGNode *new_child) {
    if (!self)
        return;
    struct DAGNode **child = &self->children;
    while (*child) {
        if (*child == new_child)
            return;
        child = &(*child)->sibling;
    }
    *child = new_child;
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

static int get_ancestor_index(struct DAGNode *self, struct DAGNode *ancestor) {
    if (!self)
        return -1;
    for (int i = 0; i < self->ancestor_amount; i++) {
        if (self->ancestor[i] == ancestor) {
            return i;
        }
    }
    return -1;
}

bool is_ancestor_of(struct DAGNode *self, struct DAGNode *child) {
    if (!child || !self)
        return false;
    return get_ancestor_index(child, self) >= 0;
}

unsigned int ancestor_count(struct DAGNode *self) {
    if (!self)
        return 0;
    return self->ancestor_count;
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

bool init_node(struct DAGNode *self, unsigned int ancestor_amount,
               struct DAGop *op, va_list *ancestors) {

    self->ancestor_amount = ancestor_amount;
    self->ancestor_count = 0;
    self->op = op;
    self->children = NULL;
    self->sibling = NULL;

    for (int i = 0; i < ancestor_amount; i++) {
        self->ancestor[i] = NULL;
    }
    return add_ancestors(self, ancestors);
}

static void remove_ancestor(struct DAGNode *self, struct DAGNode *ancestor) {
    if (!self)
        return;
    int index = get_ancestor_index(self, ancestor);
    if (index >= 0) {
        self->ancestor_count -= 1;
        self->ancestor[index] = NULL;
    }
}

bool remove_node(struct DAGNode *self) {
    if (!self || has_ancestor(self))
        return false;

    for_each_child(self, child) {
        remove_ancestor(child, self);
        if (!has_ancestor(child)) {
            if (child->op->on_all_anecestors_freed)
                child->op->on_all_anecestors_freed(child);
        }
    }

    for_each_child_pointer(self, child) {
        if (!has_ancestor(*child)) {
            struct DAGNode *tmp = *child;
            *child = (*child)->sibling;
            tmp->sibling = NULL;
        }
    }

    if (self->op->free)
        self->op->free(self);
    return true;
}

struct DAGNode **next_child_pointer(struct DAGNode *self,
                                    struct DAGNode **current_child) {
    if (!self || !current_child)
        return NULL;
    while (*current_child) {
        current_child = &(*current_child)->sibling;
        if (is_ancestor_of(self, *current_child))
            return current_child;
    }
    return current_child;
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

void print_node(struct DAGNode *self) {
    if (!self)
        return;
    if (self->op->print)
        self->op->print(self);
}