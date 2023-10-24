#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr)-offsetof(type, member)))

struct DAGFamily {
    struct DAGNode *ancestor;
    struct DAGNode *sybling;
};

// TODO: Maybe the structure should declare as __packed__?
#define DAGNode(name, ancestor_count)                                          \
    struct DAGNode name;                                                       \
    struct DAGFamily name##_family[ancestor_count];

// TODO: handle "init_node" failure
#define DAGNodeInit(name, op, last)                                            \
    do {                                                                       \
        va_list last##ancestors;                                               \
        va_start(last##ancestors, last);                                       \
        init_node(&(name), sizeof(name##_family) / sizeof(name##_family[0]),   \
                  (op), &(last##ancestors));                                   \
    } while (0)

#define for_each_child_saved(self, child, saved)                               \
    for (struct DAGNode *child = (self)->children, *saved = (self)->children;  \
         child; child = next_child(self, child), saved = (self)->children)
#define for_each_child(self, child)                                            \
    for (struct DAGNode *child = (self)->children; child;                      \
         child = next_child(self, child))

#define for_each_child_pointer(self, child)                                    \
    for (struct DAGNode **child = &(self)->children; *child;                   \
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