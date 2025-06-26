#include "../../lv_examples.h"
#if LV_USE_LABEL && LV_USE_TEXTAREA && LV_FONT_SIMSUN_16_CJK && LV_USE_IME_PINYIN && LV_BUILD_EXAMPLES

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);

    if(code == LV_EVENT_FOCUSED) {  // 输入框获得聚焦 --> 显示键盘
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);  // 清理键盘的隐藏标记 --> 显示键盘
            lv_keyboard_set_textarea(kb, ta); // 再次明确键盘的输入目标输入框
            lv_obj_align_to(ta, kb, LV_ALIGN_OUT_TOP_MID, 0, 0);
        }
    }
    else if (code == LV_EVENT_DEFOCUSED)  // 输入框失去聚焦 --> 隐藏键盘
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN); // 让键盘隐藏

        lv_obj_align(ta, LV_ALIGN_BOTTOM_MID, 0, 0);

    }
    else if(code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
    
}

void lv_example_ime_pinyin_1(void)
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE); // 清除可滚动的标志

    // 创建一个拼音对象
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(lv_scr_act());
    lv_obj_set_style_text_font(pinyin_ime, &lv_font_simsun_16_cjk, 0);
    //lv_ime_pinyin_set_dict(pinyin_ime, your_dict); // Use a custom dictionary. If it is not set, the built-in dictionary will be used.

    
    /*Create a keyboard and add it to ime_pinyin*/
    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());
    lv_ime_pinyin_set_keyboard(pinyin_ime, kb); // 把拼音键盘和拼音对象关联起来
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN); // 让键盘隐藏
    
    
    
    /* ta1 */
    lv_obj_t * ta1 = lv_textarea_create(lv_scr_act());
    lv_textarea_set_one_line(ta1, true);
    lv_obj_set_style_text_font(ta1, &lv_font_simsun_16_cjk, 0);
    lv_obj_align(ta1, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_align_to(ta1, kb, LV_ALIGN_OUT_TOP_MID, 0, 0);
    


    lv_keyboard_set_textarea(kb, ta1); // 设置键盘的目标输入框
    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);

    /*Get the cand_panel, and adjust its size and position*/
    lv_obj_t * cand_panel = lv_ime_pinyin_get_cand_panel(pinyin_ime);
    lv_obj_set_size(cand_panel, LV_PCT(100), LV_PCT(10));
    lv_obj_align_to(cand_panel, kb, LV_ALIGN_OUT_TOP_MID, 0, 0);


    // lv_obj_t * btn = lv_btn_create( lv_scr_act() );
    // lv_obj_set_width(btn, 100);
    // lv_obj_set_height(btn, 50);

}



#endif
