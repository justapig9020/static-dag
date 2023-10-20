#include "custom_dags.h"
#include <stdio.h>
#include <stdlib.h>

#define VNODE_AMOUNT 10
#define SNODE_AMOUNT 10

static struct VNode vnode_pool[VNODE_AMOUNT];
static struct SNode snode_pool[SNODE_AMOUNT];

static int vnode_pool_index = 0;
static int snode_pool_index = 0;

static void snode_print(struct DAGNode *n) {
    struct SNode *snode = container_of(n, struct SNode, node);
    printf("SNODE %s\n", snode->name);
}

static void vnodes_print(struct DAGNode *n) {
    struct VNode *vnode = container_of(n, struct VNode, node);
    printf("VNODE %d\n", vnode->data);
}

struct VNode *new_vnode(int data) {
    printf("new vnode %d\n", data);
    if (vnode_pool_index >= VNODE_AMOUNT) {
        printf("new vnode failed\n");
        return NULL;
    }
    struct VNode *vnode = &vnode_pool[vnode_pool_index];
    vnode_pool_index += 1;
    vnode->data = data;
    DAGNodeInit(vnode->node, vnodes_print);
    printf("new vnode success\n");
    return vnode;
}

struct SNode *new_snode(char *name) {
    printf("new snode %s\n", name);
    if (snode_pool_index >= SNODE_AMOUNT) {
        printf("new snode failed\n");
        return NULL;
    }
    struct SNode *snode = &snode_pool[snode_pool_index];
    snode_pool_index += 1;
    snode->name = name;
    DAGNodeInit(snode->node, snode_print);
    printf("new snode success\n");
    return snode;
}