#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: default_btn
//

void init_style_default_btn_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff4c4c4c));
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_margin_left(style, 10);
    lv_style_set_margin_top(style, 10);
};

lv_style_t *get_style_default_btn_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_default_btn_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_default_btn_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffbbbbbb));
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
};

lv_style_t *get_style_default_btn_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_default_btn_MAIN_FOCUSED(style);
    }
    return style;
};

void add_style_default_btn(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_default_btn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_default_btn_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

void remove_style_default_btn(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_default_btn_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_default_btn_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_default_btn,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_default_btn,
    };
    remove_style_funcs[styleIndex](obj);
}

