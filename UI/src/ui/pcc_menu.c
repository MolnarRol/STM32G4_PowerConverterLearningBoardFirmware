/*
 * pcc_menu.c
 *
 *  Created on: Mar 4, 2025
 *      Author: molnar
 */
#include "screens.h"
#include "actions.h"
#include "PCC_private_interface.h"
#include "styles.h"

extern lv_indev_t *            input_encoder_ps;
extern lv_indev_t *            input_push_btn_ps;

extern const PCC_TopologyHandle_struct* const s_PCC_Topologies_aps[];

void ui_init_pcc_menu_v(void)
{

    s32 topo_s32;
    lv_obj_t* button_p;
    lv_obj_t* btn_label_p;

    lv_obj_clean(lv_screen_active());

    for(topo_s32 = 0; topo_s32 < 14; topo_s32++)
    {
        button_p = lv_button_create(lv_screen_active());
        lv_obj_set_size(button_p, LV_PCT(90), 30);
        lv_obj_set_pos(button_p, LV_PCT(5), 40 * topo_s32);
        lv_obj_add_event_cb(button_p, action_load_pcc_topology_ctrl_screen, LV_EVENT_CLICKED, (void*)s_PCC_Topologies_aps[topo_s32]);

        btn_label_p = lv_label_create(button_p);
        lv_label_set_text(btn_label_p, s_PCC_Topologies_aps[topo_s32]->ctrl_name_str);
        lv_obj_center(btn_label_p);
        lv_group_add_obj(groups.pcc_topology_select_grp, button_p);

        add_style_default_btn(button_p);
//        lv_obj_set_style_bg_color(button_p, lv_color_hex(0xffbbbbbb), LV_PART_MAIN | LV_STATE_FOCUSED);
//        lv_obj_set_style_text_color(button_p, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_FOCUSED);
    }
}
