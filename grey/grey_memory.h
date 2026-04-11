#ifndef GREY_MEMORY_H
#define GREY_MEMORY_H

#include <stdbool.h>
#include <stdlib.h>

#include "type_alias.h"

static inline mem gmalloc(u64 size) { return malloc(size); }
static inline mem gcalloc(u64 count, u64 size) { return calloc(count, size); }
static inline bool gfree(mem addr) {
  free(addr);
  return true;
}

#endif
