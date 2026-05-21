#ifndef GREY_EVENTS_H
#define GREY_EVENTS_H

#include "arena.h"
#include "grey_ecs.h"
#include "type_alias.h"

#define GREY_PAYLOAD_SIZE (32)
#define GREY_MAX_EVENTS (1024)

typedef struct {
  u32 trigger_id;
  u32 map_grid_idx;
  Entity entity;
  Entity other;
} GreyTrigger;

typedef enum { NO_EVENT, TRIGGER_EVENT, NUM_EVENT } GreyEventType;

typedef struct {
  GreyEventType type;
  union {
    GreyTrigger trigger;
    u8 game_payload[GREY_PAYLOAD_SIZE];
  } as;
} GreyEvent;

typedef struct grey_event_system {
  Arena arena;
  GreyEvent *event;
  u32 n_events;
  u32 cap;
} *GreyEvents;

GreyEvents event_system_create(Arena arena);
void event_system_add_event(GreyEvents system, GreyEvent event);
u32 event_system_get_event_count(GreyEvents system);
GreyEvent *event_system_get_events(GreyEvents system);
void event_system_reset(GreyEvents system);

#endif
