#include "custom_dags.h"
#define DEBUG // uncomment this line to enable debug output
#include "dag.h"
#include "debug.h"
#include <stddef.h>

int main(void) {
    struct DAGNode *a, *b, *c, *d, *ab, *cd, *bc, *ad;

    a = as_node(new_snode("a"));
    b = as_node(new_snode("b"));
    c = as_node(new_snode("c"));
    d = as_node(new_snode("d"));
    ab = as_node(new_snode("ab", a, b, NULL));
    cd = as_node(new_snode("cd", c, d, NULL));
    bc = as_node(new_snode("bc", b, c, NULL));
    ad = as_node(new_snode("ad", a, d, NULL));

    print_dag();

    struct DAGNode *remove[] = {b, c, bc, a, d, ab, cd, ad};

    for (int i = 0; i < sizeof(remove) / sizeof(remove[0]); i++) {
        debug("Removing: ");
        print_node(remove[i]);
        remove_node(remove[i]);
        print_dag();
    }
    return 0;
}