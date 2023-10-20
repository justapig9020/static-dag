#include "custom_dags.h"
#include "dag.h"
#include <stddef.h>

int main(void) {
    struct DAGNode *v0, *v1, *v2, *v3, *v4, *s1, *s2;

    v0 = as_node(new_vnode(0));
    v1 = as_node(new_vnode(1));
    v2 = as_node(new_vnode(2));
    v3 = as_node(new_vnode(3));
    v4 = as_node(new_vnode(4));

    s1 = as_node(new_snode("s1"));
    s2 = as_node(new_snode("s2"));

    v2->op->add_ancestor(v2, v0);
    v2->op->add_ancestor(v2, v1);

    s1->op->add_ancestor(s1, v0);
    s1->op->add_ancestor(s1, v1);
    s1->op->add_ancestor(s1, v3);

    v4->op->add_ancestor(v4, v2);
    v4->op->add_ancestor(v4, s1);

    s2->op->add_ancestor(s2, v2);
    s2->op->add_ancestor(s2, v3);
    s2->op->add_ancestor(s2, s1);

    print_dag(v0);
    print_dag(v1);
    print_dag(v2);
    return 0;
}