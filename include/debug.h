#pragma once
#include <stdio.h>

#ifdef DEBUG
#define debug(...)                                                             \
  do {                                                                         \
    fprintf(stderr, __VA_ARGS__);                                              \
  } while (0)
#else
#define debug(...)
#endif
