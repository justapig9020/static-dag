#include "custom_dags.h"
#define DEBUG // uncomment this line to enable debug output
#include "dag.h"
#include "debug.h"
#include <stddef.h>

int main(void) {
  struct DAGNode *a, *b, *c, *d, *ab, *cd, *bc, *ad;

  a = as_node(new_snode("a", NULL, NULL, NULL));
  b = as_node(new_snode("b", NULL, NULL, NULL));
  c = as_node(new_snode("c", NULL, NULL, NULL));
  d = as_node(new_snode("d", NULL, NULL, NULL));
  ab = as_node(new_snode("ab", a, b, a));
  cd = as_node(new_snode("cd", c, d, c));
  bc = as_node(new_snode("bc", b, c, c));
  ad = as_node(new_snode("ad", a, d, d));

  print_dag();

  struct DAGNode *remove[] = {b, c, bc, a, d, ab, cd, ad};

  for (int i = 0; i < sizeof(remove) / sizeof(remove[0]); i++) {
    debug("Remove: ");
    print_node(remove[i]);
    remove_node(remove[i]);
    print_dag();
  }
  return 0;
}
