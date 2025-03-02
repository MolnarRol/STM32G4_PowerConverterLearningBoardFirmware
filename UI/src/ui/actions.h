#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_go_to_power_topology_menu(lv_event_t * e);
extern void action_change_active_state_of_frequency_parameters(lv_event_t * e);
extern void action_back_to_main_menu(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/