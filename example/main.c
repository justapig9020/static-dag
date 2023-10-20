#include "custom_dags.h"
#define DEBUG
#include "dag.h"
#include "debug.h"
#include <stddef.h>

int main(void) {
    struct DAGNode *root, *v0, *v1, *v2, *v3, *v4, *s1, *s2;

    root = as_node(new_snode("root"));
    v0 = as_node(new_vnode(0, root, NULL));
    v1 = as_node(new_vnode(1, root, NULL));
    v2 = as_node(new_vnode(2, v0, v1, NULL));
    v3 = as_node(new_vnode(3, root, NULL));
    s1 = as_node(new_snode("s1", v0, v1, v3, NULL));
    v4 = as_node(new_vnode(4, v2, s1, NULL));
    s2 = as_node(new_snode("s2", v2, v3, s1, NULL));

    print_dag();

    struct DAGNode *list[] = {root, v0, v1, v3, v2, s1, v4, s2};
    for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
        remove_node(list[i]);
        print_dag();
    }
    return 0;
}