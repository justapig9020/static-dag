#include "custom_dags.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#include "debug.h"

#define VNODE_AMOUNT 10
#define SNODE_AMOUNT 10

#include <stdbool.h>

struct VNodeChunk {
    struct VNode node;
    bool allocated;
};

struct SNodeChunk {
    struct SNode node;
    bool allocated;
};
static struct VNodeChunk vnode_pool[VNODE_AMOUNT];
static struct SNodeChunk snode_pool[SNODE_AMOUNT];

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
    int i;
    for (i = 0; i < VNODE_AMOUNT; i++) {
        if (!vnode_pool[i].allocated) {
            struct VNode *vnode = &vnode_pool[i].node;
            vnode_pool[i].allocated = true;
            vnode->data = data;

            DAGNodeInit(vnode->node, &vnode_op, data);
            return vnode;
        }
    }
    return NULL;
}

struct SNode *new_snode(char *name, ...) {
    int i;
    for (i = 0; i < SNODE_AMOUNT; i++) {
        if (!snode_pool[i].allocated) {
            struct SNode *snode = &snode_pool[i].node;
            snode_pool[i].allocated = true;
            snode->name = name;

            DAGNodeInit(snode->node, &snode_op, name);
            return snode;
        }
    }
    return NULL;
}

void free_vnode(struct DAGNode *n) {
    struct VNode *vnode = container_of(n, struct VNode, node);
    struct VNodeChunk *chunk = container_of(vnode, struct VNodeChunk, node);
    chunk->allocated = false;
}

void free_snode(struct DAGNode *n) {
    struct SNode *snode = container_of(n, struct SNode, node);
    struct SNodeChunk *chunk = container_of(snode, struct SNodeChunk, node);
    chunk->allocated = false;
}

void all_ancestors_freed(struct DAGNode *n) {
    print_node(n);
    debug("all ancestors freed\n");
}

static struct DAGop vnode_op = {.free = free_vnode,
                                .print = vnode_print,
                                .on_all_anecestors_freed = all_ancestors_freed};
static struct DAGop snode_op = {.free = free_snode,
                                .print = snode_print,
                                .on_all_anecestors_freed = all_ancestors_freed};

static void print_vnode(struct VNode *vnode) {
    struct DAGNode *node = &vnode->node;
    debug("%p[\"%d: %d\"];\n", node, vnode->data, ancestor_count(node));
    for_each_child(node, child) { debug("%p --> %p;\n", node, child); }
}

static void print_snode(struct SNode *snode) {
    struct DAGNode *node = &snode->node;
    debug("%p[\"%s: %d\"];\n", node, snode->name, ancestor_count(node));
    for_each_child(node, child) { debug("%p --> %p;\n", node, child); }
}

void print_dag() {
    debug("```mermaid\n");
    debug("flowchart LR;\n");
    for (int i = 0; i < VNODE_AMOUNT; i++) {
        if (vnode_pool[i].allocated) {
            print_vnode(&vnode_pool[i].node);
        }
    }
    for (int i = 0; i < SNODE_AMOUNT; i++) {
        if (snode_pool[i].allocated) {
            print_snode(&snode_pool[i].node);
        }
    }
    debug("```\n");
}