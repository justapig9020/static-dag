#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr)-offsetof(type, member)))

// TODO: Maybe the structure should declare as __packed__?
#define DAGNode(name, ancestor_count)                                          \
    struct DAGNode name;                                                       \
    struct DAGNode *name##_ancestor[ancestor_count];

// TODO: handle "init_node" failure
#define DAGNodeInit(name, op, last)                                            \
    do {                                                                       \
        va_list last##ancestors;                                               \
        va_start(last##ancestors, last);                                       \
        init_node(&(name),                                                     \
                  sizeof(name##_ancestor) / sizeof(name##_ancestor[0]), (op),  \
                  &(last##ancestors));                                         \
    } while (0)

#define for_each_child(self, child)                                            \
    for (struct DAGNode *child = (self)->children; child;                      \
         child = next_child(self, child))

#define for_each_child_pointer(self, child)                                    \
    for (struct DAGNode **child = &(self)->children; *child;                   \
         child = next_child_pointer(self, child))

struct DAGNode {
    struct DAGop *op;
    struct DAGNode *children;
    struct DAGNode *sibling;
    unsigned int ancestor_amount;
    unsigned int ancestor_count;
    struct DAGNode *ancestor[0];
};

struct DAGop {
    void (*free)(struct DAGNode *self);
    void (*print)(struct DAGNode *self);
    void (*on_all_anecestors_freed)(struct DAGNode *self);
};

bool init_node(struct DAGNode *self, unsigned int ancestor_amount,
               struct DAGop *op, va_list *ancestors);
bool remove_node(struct DAGNode *self);

bool has_ancestor(struct DAGNode *self);
bool is_ancestor_of(struct DAGNode *self, struct DAGNode *child);
unsigned int ancestor_count(struct DAGNode *self);
struct DAGNode **next_child_pointer(struct DAGNode *self,
                                    struct DAGNode **current_child);
struct DAGNode *next_child(struct DAGNode *self, struct DAGNode *current_child);
void print_node(struct DAGNode *self);