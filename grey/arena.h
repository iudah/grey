#ifndef ARENA_H
#define ARENA_H

#include "type_alias.h"

typedef struct arena *Arena;

Arena arena_create(u64 size);
void arena_destroy(Arena arena);
mem arena_allocate(Arena arena, u64 size);
void arena_reset(Arena arena);

#endif
