#include "dag.h"

struct VNode {
    int data;
    DAGNode(node, 2);
};

struct SNode {
    char *name;
    DAGNode(node, 3);
};

#define as_node(self) ((struct DAGNode *)(&self->node))
struct VNode *new_vnode(int data, ...);
struct SNode *new_snode(char *name, ...);
void print_dag();