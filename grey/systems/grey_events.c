#include "grey_events.h"

GreyEvents event_system_create(Arena arena) {
  GreyEvents event_system = arena_allocate(arena, sizeof(*event_system));
  event_system->arena = arena;
  event_system->n_events = 0;
  event_system->cap = GREY_MAX_EVENTS;
  event_system->event = arena_allocate(
      event_system->arena, event_system->cap * sizeof(*event_system->event));

  return event_system;
}

void event_system_add_event(GreyEvents system, GreyEvent event) {
  if (system->n_events >= system->cap) {
    return;
  }
  system->event[system->n_events++] = event;
}

u32 event_system_get_event_count(GreyEvents system) { return system->n_events; }
GreyEvent *event_system_get_events(GreyEvents system) { return system->event; }
void event_system_reset(GreyEvents system) { system->n_events = 0; }
