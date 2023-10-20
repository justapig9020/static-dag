#include "custom_dags.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#include "debug.h"

#define VNODE_AMOUNT 10
#define SNODE_AMOUNT 10

static struct VNode vnode_pool[VNODE_AMOUNT];
static struct SNode snode_pool[SNODE_AMOUNT];

static int vnode_pool_index = 0;
static int snode_pool_index = 0;

static struct DAGop vnode_op;
static struct DAGop snode_op;

static void snode_print(struct DAGNode *n) {
    struct SNode *snode = container_of(n, struct SNode, node);
    debug("%p[\"%s\"];\n", n, snode->name);
}

static void vnode_print(struct DAGNode *n) {
    struct VNode *vnode = container_of(n, struct VNode, node);
    debug("%p[\"%d\"];\n", n, vnode->data);
}

struct VNode *new_vnode(int data, ...) {
    if (vnode_pool_index >= VNODE_AMOUNT) {
        return NULL;
    }
    struct VNode *vnode = &vnode_pool[vnode_pool_index];
    vnode_pool_index += 1;
    vnode->data = data;

    DAGNodeInit(vnode->node, &vnode_op, data);
    return vnode;
}

struct SNode *new_snode(char *name, ...) {
    if (snode_pool_index >= SNODE_AMOUNT) {
        return NULL;
    }
    struct SNode *snode = &snode_pool[snode_pool_index];
    snode_pool_index += 1;
    snode->name = name;
    DAGNodeInit(snode->node, &snode_op, name);
    return snode;
}

static struct DAGop vnode_op = {.print = vnode_print};
static struct DAGop snode_op = {.print = snode_print};