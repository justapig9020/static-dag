#include "custom_dags.h"
// #define DEBUG // uncomment this line to enable debug output
#include "dag.h"
#include "debug.h"
#include <stddef.h>

int main(void) {
    struct DAGNode *v0, *v1, *v2, *v3, *v4, *s1, *s2;

    v0 = as_node(new_vnode(0, NULL));
    v1 = as_node(new_vnode(1, NULL));
    v2 = as_node(new_vnode(2, v0, v1, NULL));
    v3 = as_node(new_vnode(3, NULL));
    s1 = as_node(new_snode("s1", v0, v1, v3, NULL));
    v4 = as_node(new_vnode(4, v2, s1, NULL));
    s2 = as_node(new_snode("s2", v2, v3, s1, NULL));

    print_dag();

    {
        struct DAGNode *list[] = {v0, v1, v3, v2, s1, v4, s2};
        for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
            remove_node(list[i]);
            print_dag();
        }
    }

    struct DAGNode *a, *b;
    a = as_node(new_snode("a", NULL));
    b = as_node(new_snode("b", NULL));
    v1 = as_node(new_vnode(1, a, NULL));
    v3 = as_node(new_vnode(3, a, b, NULL));
    v2 = as_node(new_vnode(2, a, b, NULL));
    print_dag();

    {
        struct DAGNode *list[] = {a, b, v1, v2, v3};
        for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
            remove_node(list[i]);
            print_dag();
        }
    }

    return 0;
}