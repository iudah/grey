#include "game_anim.h"
#include "grey_events.h"

void game_system_trigger_update(GreyEvents system, u8 **trigger_cell) {
  auto n_events = event_system_get_event_count(system);
  auto events = event_system_get_events(system);

  for (u32 i = 0; i < n_events; ++i) {
    auto event = events[i];
    if (event.type == TRIGGER_EVENT) {
      if (event.as.trigger.trigger_id == WATER_TILE) {
      }
      if (event.as.trigger.trigger_id == ONE_WAY_ENTRY_TRIGGER) {
        auto actor = event.as.trigger.entity;
        auto trigger = event.as.trigger.other;

        *trigger_cell[trigger] = 1;
      }
      if (event.as.trigger.trigger_id == ONE_WAY_EXIT_TRIGGER) {
        auto actor = event.as.trigger.entity;
        auto trigger = event.as.trigger.other;

        *trigger_cell[trigger] = 2;
      }
    }
  }
}
