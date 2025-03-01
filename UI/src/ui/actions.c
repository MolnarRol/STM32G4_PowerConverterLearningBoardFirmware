#include "ui.h"
#include "actions.h"
#include "screens.h"

extern lv_indev_t* encoder_input_device_ps;

void action_go_to_power_topology_menu(lv_event_t * e)
{
    lv_scr_load(objects.power_topology_menu);
    lv_indev_set_group(encoder_input_device_ps, groups.PCC_topo_select_group);
}
