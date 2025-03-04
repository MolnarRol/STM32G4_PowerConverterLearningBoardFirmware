#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_go_to_power_topology_menu(lv_event_t * e);
extern void action_back_to_main_menu(lv_event_t * e);
extern void action_pcc_topology_menu_loaded(lv_event_t * e);
extern void action_load_pcc_topology_ctrl_screen(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/