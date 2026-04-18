#include "arena.h"
#include "grey_assert.h"
#include "grey_memory.h"

#define ALIGNMENT (sizeof(mem))
#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

struct arena {
  char *buffer;
  u64 size;
  u64 top;
};

Arena arena_create(u64 size) {
  Arena arena = gmalloc(sizeof(*arena));

  size = ALIGN(size);

  arena->size = size;
  arena->buffer = gmalloc(size);
  arena->top = 0;

  return arena;
}

void arena_destroy(Arena arena) {
  GREY_ASSERT(arena, "Attempt to destroy NULL arena.");
  gfree(arena->buffer);
  gfree(arena);
}

mem arena_allocate(Arena arena, u64 size) {
  GREY_ASSERT(arena, "Attempt to allocate NULL arena.");

  size = ALIGN(size);
  u64 next_top = arena->top + size;

  if (next_top > arena->size)
    return NULL;

  u64 top = arena->top;
  arena->top = next_top;

  return (mem)(arena->buffer + top);
}

void arena_reset(Arena arena) {
  GREY_ASSERT(arena, "Attempt to reset NULL arena.");
  arena->top = 0;
}
