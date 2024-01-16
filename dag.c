#include "dag.h"
#include <stdio.h>

// #define DEBUG // uncomment this line to enable debug output
#include "debug.h"

bool add_ancestor(struct DAGNode *self, struct DAGNode *ancestor) {
  if (!self)
    return false;
  if (self->ancestor_count >= self->ancestor_amount) {
    return false;
  }
  unsigned int index = self->ancestor_count;
  self->family[index].ancestor = ancestor;
  self->family[index].sybling = ancestor->children;
  self->ancestor_count += 1;
  ancestor->children = self;
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
    if (self->family[i].ancestor == ancestor) {
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

static bool add_ancestors(struct DAGNode *self, int num, va_list *ancestors) {
  for (int i = 0; i < num; i++) {
    struct DAGNode *ancestor = va_arg(*ancestors, struct DAGNode *);
    bool success = add_ancestor(self, ancestor);
    if (!success)
      // TODO: clean up added ancestors
      return false;
  }
  return true;
}

void init_node(struct DAGNode *self, unsigned int ancestor_amount,
               struct DAGop *op) {

  self->ancestor_amount = ancestor_amount;
  self->ancestor_count = 0;
  self->op = op;
  self->children = NULL;

  for (int i = 0; i < ancestor_amount; i++) {
    self->family[i].ancestor = NULL;
    self->family[i].sybling = NULL;
  }
}

static void remove_ancestor(struct DAGNode *self, struct DAGNode *ancestor) {
  if (!self)
    return;
  int index = get_ancestor_index(self, ancestor);
  if (index < 0)
    return;
  self->ancestor_count -= 1;
  self->family[index].ancestor = NULL;
}

bool remove_node(struct DAGNode *self) {
  if (!self || has_ancestor(self))
    return false;

  struct DAGNode **child = &self->children;
  while (*child) {
    struct DAGNode *curr = *child;
    *child = next_child(self, curr);
    remove_ancestor(curr, self);
    if (!has_ancestor(curr)) {
      if (curr->op->on_all_anecestors_freed)
        curr->op->on_all_anecestors_freed(curr);
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
  int index = get_ancestor_index(*current_child, self);
  if (index < 0)
    return NULL;
  struct DAGNode **next_child = &((*current_child)->family[index].sybling);
  return next_child;
}

struct DAGNode *next_child(struct DAGNode *self,
                           struct DAGNode *current_child) {
  if (!self || !current_child)
    return NULL;
  int index = get_ancestor_index(current_child, self);
  if (index < 0)
    return NULL;
  struct DAGNode *next_child = current_child->family[index].sybling;
  return next_child;
}

void print_node(struct DAGNode *self) {
  if (!self)
    return;
  if (self->op->print)
    self->op->print(self);
}