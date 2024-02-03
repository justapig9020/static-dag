#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define sizeof_dag_node(node)                                                  \
  (sizeof(node) + sizeof(struct DAGFamily) * (node).ancestor_amount)

#ifndef container_of
#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr)-offsetof(type, member)))
#endif

struct DAGFamily {
  struct DAGNode *ancestor;
  struct DAGNode *sybling;
};

// TODO: Maybe the structure should declare as __packed__?
#define DAGNode(name, ancestor_count)                                          \
  struct DAGNode name;                                                         \
  struct DAGFamily name##_family[ancestor_count];

// TODO: handle "init_node" failure
#define DAGNodeInit(name, op)                                                  \
  do {                                                                         \
    init_node(&(name), sizeof(name##_family) / sizeof(name##_family[0]),       \
              (op));                                                           \
  } while (0)

#define for_each_child_saved(self, child, saved)                               \
  for (struct DAGNode *child = (self)->children, *saved = (self)->children;    \
       child; child = next_child(self, child), saved = (self)->children)

#define for_each_child(self, child)                                            \
  for (struct DAGNode *child = (self)->children; child;                        \
       child = next_child(self, child))

#define for_each_child_pointer(self, child)                                    \
  for (struct DAGNode **child = &(self)->children; *child;                     \
       child = next_child_pointer(self, child))

struct DAGNode {
  struct DAGop *op;
  struct DAGNode *children;
  unsigned int ancestor_amount;
  unsigned int ancestor_count;
  struct DAGFamily family[0];
};

struct DAGop {
  void (*free)(struct DAGNode *self);
  void (*print)(struct DAGNode *self);
  void (*on_all_anecestors_freed)(struct DAGNode *self);
};

// Initialize the given node "self".
// The ancestor_amount indicates the maximum number of ancestor the node "self".
// The op is the operation of the node "self".
void init_node(struct DAGNode *self, unsigned int ancestor_amount,
               struct DAGop *op);

// Add the node "ancestor" as an ancestor of the node "self".
// If there has enough space for node "self" to add the ancestor, the node
// "ancestor" is successfully added to node "self" and the function returns
// true, includes node "ancestor" is already ancestor of node "self". Otherwire,
// return false. Special case: You can use a NULL pointer to indicates an empty
// node and the function returns true in this case.
bool add_ancestor(struct DAGNode *self, struct DAGNode *ancestor);

// Remove the node "self" from the DAG. The remove success if the node has no
// ancestor.
// Return true if the node "self" is removed from the DAG. Otherwire,
// return false.
bool remove_node(struct DAGNode *self);

// Weither the node "self" has ancestor.
// Return true if the node "self" has ancestor. Otherwire, return false.
bool has_ancestor(struct DAGNode *self);

// Weither the node "self" is ancestor of the node "child".
// Return true if the node "self" is ancestor of the node "child". Otherwire,
// return false.
bool is_ancestor_of(struct DAGNode *self, struct DAGNode *child);

// Return the number of ancestor of the node "self".
unsigned int ancestor_count(struct DAGNode *self);

// Return a pointer to a pointer which owned by "current_child" and the pointer
// is pointing to the next sibling of "current_child" which also has "self" as
// its ancestor.
// If there is no sibling of "current_child" which has "self" as ancestor, the
// function returns NULL.
// The function is used to iterate the children of the node "self".
struct DAGNode **next_child_pointer(struct DAGNode *self,
                                    struct DAGNode **current_child);

// Return the pointer to the next sibling of "current_child" which also has
// "self" as its ancestor.
// If there is no such sibling exists, the function returns NULL.
// The function is used to iterate the children of the node "self".
struct DAGNode *next_child(struct DAGNode *self, struct DAGNode *current_child);
void print_node(struct DAGNode *self);

#ifdef __cplusplus
}
#endif
