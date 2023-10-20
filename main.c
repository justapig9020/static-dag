#include "custom_dags.h"
#include "dag.h"
#include <stddef.h>

int main(void) {
    struct DAGNode *root, *v0, *v1, *v2, *v3, *v4, *s1, *s2;

    root = as_node(new_snode("root"));
    v0 = as_node(new_vnode(0));
    v1 = as_node(new_vnode(1));
    v2 = as_node(new_vnode(2));
    v3 = as_node(new_vnode(3));
    v4 = as_node(new_vnode(4));

    s1 = as_node(new_snode("s1"));
    s2 = as_node(new_snode("s2"));

    add_ancestor(v0, root);
    add_ancestor(v1, root);
    add_ancestor(v3, root);

    add_ancestor(v2, v0);
    add_ancestor(v2, v1);

    add_ancestor(s1, v0);
    add_ancestor(s1, v1);
    add_ancestor(s1, v3);

    add_ancestor(v4, v2);
    add_ancestor(v4, s1);

    add_ancestor(s2, v2);
    add_ancestor(s2, v3);
    add_ancestor(s2, s1);

    print_dag(root);
    return 0;
}