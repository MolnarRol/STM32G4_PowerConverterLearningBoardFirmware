#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: default_btn
lv_style_t *get_style_default_btn_MAIN_DEFAULT();
lv_style_t *get_style_default_btn_MAIN_FOCUSED();
void add_style_default_btn(lv_obj_t *obj);
void remove_style_default_btn(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/