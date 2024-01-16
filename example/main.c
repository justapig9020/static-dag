#include "custom_dags.h"
#define DEBUG // uncomment this line to enable debug output
#include "dag.h"
#include "debug.h"
#include <stddef.h>

int main(void) {
  struct DAGNode *a, *b, *c, *d, *ab, *cd, *bc, *ad;

  a = as_node(new_snode("a", 0));
  b = as_node(new_snode("b", 0));
  c = as_node(new_snode("c", 0));
  d = as_node(new_snode("d", 0));
  ab = as_node(new_snode("ab", 2, a, b));
  cd = as_node(new_snode("cd", 2, c, d));
  bc = as_node(new_snode("bc", 2, b, c));
  ad = as_node(new_snode("ad", 2, a, d));

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