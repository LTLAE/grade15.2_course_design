#include "../ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// date time etc global
extern char DATE_STR[6]; // MM/DD format
extern char DAY_STR[4]; // Day format
extern char TIME_STR[6]; // HH:MM format
extern char *CITY_NAME;
extern char *WEATHER_MAIN;
extern char *TEMPERATURE;
extern char *ADVICE;

int get_weather(const char *);

static void go_to_screen1()
{
    printf("go_to_screen1: Switching to Screen1...\n");
    ui_Screen1_screen_init();
    lv_scr_load(ui_Screen1);
}

static void button_on_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        printf("button_on_click: Button clicked!\n");
        go_to_screen1();
    }
}

static void input_city_area_events(lv_event_t * e)
{
    lv_event_code_t input_city_area_status = lv_event_get_code(e);
    lv_obj_t * keyboard = lv_event_get_user_data(e);

    // code from teacher demo
    if(input_city_area_status == LV_EVENT_FOCUSED) {  // when input_city area focused (clicked)
        printf("input_city_area_events: input_city_area focused.\n");
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);  // 清理键盘的隐藏标记 --> 显示键盘
            // lv_obj_clear_flag(pinyin_ime, LV_OBJ_FLAG_HIDDEN);  // no pinyin
            lv_keyboard_set_textarea(keyboard, input_city_area); // 再次明确键盘的输入目标输入框
        }
    }

    else if (input_city_area_status == LV_EVENT_DEFOCUSED)  // when input_city area not focused (clicked outside)
    {
        printf("input_city_area_events: input_city_area defocused.\n");
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // 让键盘隐藏
        // lv_obj_add_flag(pinyin_ime, LV_OBJ_FLAG_HIDDEN); // 让键盘隐藏  // no pinyin
        // lv_obj_add_flag(cand_panel, LV_OBJ_FLAG_HIDDEN);// 让候选框隐藏  // no pinyin

    }

}

void keyboard_event(lv_event_t * e)
{
    lv_event_code_t keyboard_status = lv_event_get_code(e);
    lv_obj_t * keyboard = lv_event_get_target(e);
    lv_obj_t * input_city_area = lv_keyboard_get_textarea(keyboard);

    if(keyboard_status == LV_EVENT_READY) {   // when √ pressed     
        printf("Keyboard event: Enter pressed. Make keyboard hidden.\n");  
        // !! keyboard would not hide, fix it !!      
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // 让键盘隐藏
        // lv_obj_add_flag(pinyin_ime, LV_OBJ_FLAG_HIDDEN);// 让候选框隐藏 // no pinyin
        // lv_obj_add_flag(cand_panel, LV_OBJ_FLAG_HIDDEN);// 让候选框隐藏 // no pinyin
        lv_obj_clear_flag(ui_Getting_data_text, LV_OBJ_FLAG_HIDDEN); // show "Getting data..."
        lv_refr_now(NULL);  // refresh ui to solve keyboard not hiding issue
        const char *city_name = lv_textarea_get_text(input_city_area);
        printf("Keyboard event: City name entered: %s, try to get weather data...\n", city_name);
        // try to get weather data 3 times, if failed, return to screen 1
        for (int timer = 0; timer < 3; timer++) {
            if (get_weather(city_name) == 0) { // success
                printf("Keyboard event: Weather data received successfully for city '%s'.\n", city_name);
                break;
            }
            printf("Keyboard event: Error getting weather data for city '%s'. Retrying...\n", city_name);
            sleep(1);
            // show failure message
            CITY_NAME = strdup("Failed");
            WEATHER_MAIN = strdup("N/A");
            TEMPERATURE = strdup("0");
            ADVICE = strdup("Input city name is invalid. Please try again.");
        }
        // weather data is stored in global variables and will automatically shown when initializing ui_Screen1
        printf("Keyboard event: Weather data received, switching to Screen1...\n");
        go_to_screen1();
    }
}

void ui_Screen2_screen_init(void)
{
    printf("Start initializing UI Screen2...\n");
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_clear_flag( ui_Screen2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(ui_Screen2, lv_color_hex(0x89A68A), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_Screen2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    
    printf("Initializing ui_Frame...\n");
    ui_Frame = lv_obj_create(ui_Screen2);
    lv_obj_remove_style_all(ui_Frame);
    lv_obj_set_width( ui_Frame, 800);
    lv_obj_set_height( ui_Frame, 484);
    lv_obj_set_align( ui_Frame, LV_ALIGN_CENTER );
    lv_obj_clear_flag( ui_Frame, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_border_color(ui_Frame, lv_color_hex(0x635B51), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(ui_Frame, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Frame, 20, LV_PART_MAIN| LV_STATE_DEFAULT);

    printf("Initializing ui_FrameButtom...\n");
    ui_FrameButtom = lv_obj_create(ui_Screen2);
    lv_obj_remove_style_all(ui_FrameButtom);
    lv_obj_set_width( ui_FrameButtom, 803);
    lv_obj_set_height( ui_FrameButtom, 50);
    lv_obj_set_x( ui_FrameButtom, 0 );
    lv_obj_set_y( ui_FrameButtom, 199 );
    lv_obj_set_align( ui_FrameButtom, LV_ALIGN_CENTER );
    lv_obj_clear_flag( ui_FrameButtom, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(ui_FrameButtom, lv_color_hex(0x645D52), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_FrameButtom, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    printf("Initializing ui_SettingButton...\n");
    ui_SettingButton = lv_obj_create(ui_Screen2);
    lv_obj_set_width( ui_SettingButton, 82);
    lv_obj_set_height( ui_SettingButton, 17);
    lv_obj_set_x( ui_SettingButton, 338 );
    lv_obj_set_y( ui_SettingButton, 208 );
    lv_obj_set_align( ui_SettingButton, LV_ALIGN_CENTER );
    lv_obj_clear_flag( ui_SettingButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_radius(ui_SettingButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingButton, lv_color_hex(0xC1EDD5), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_SettingButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    printf("Initializing input_city_area...\n");
    input_city_area = lv_textarea_create(ui_Screen2);
    lv_obj_set_width(input_city_area, 400);
    lv_obj_set_height(input_city_area, 50);
    lv_obj_set_x( input_city_area, 45 );
    lv_obj_set_y( input_city_area, 45 );
    lv_textarea_set_placeholder_text(input_city_area,"Click to enter city name");
    lv_textarea_set_text(input_city_area, "");
    lv_obj_set_style_text_color(input_city_area, lv_color_hex(0x44434D), LV_PART_TEXTAREA_PLACEHOLDER | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(input_city_area, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(input_city_area, &ui_font_NanoDyongSong_40, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(input_city_area, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);   // make background transparent
    lv_obj_set_style_border_width(input_city_area, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // remove border
    lv_obj_set_style_pad_all(input_city_area, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // remove padding

    printf("Initializing Getting_data_text...\n");
    ui_Getting_data_text = lv_label_create(ui_Screen2);
    lv_obj_set_width( ui_Getting_data_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_Getting_data_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_Getting_data_text, 45 );
    lv_obj_set_y( ui_Getting_data_text, 190 );
    lv_label_set_long_mode(ui_Getting_data_text, LV_LABEL_LONG_WRAP);
    lv_label_set_text(ui_Getting_data_text,"Getting data, please wait...");
    lv_obj_set_style_text_color(ui_Getting_data_text, lv_color_hex(0x44434D), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_Getting_data_text, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Getting_data_text, &ui_font_NanoDyongSong_40, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_Getting_data_text, LV_OBJ_FLAG_HIDDEN); // initially hide the text

    printf("Initializing keyboard...\n");
    lv_obj_t * keyboard = lv_keyboard_create(ui_Screen2);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // initially hide the keyboard


    // below: event handling
    printf("Setting up event handling...\n");
    // handle button click events
    lv_obj_add_event_cb(ui_SettingButton, button_on_click, LV_EVENT_ALL, NULL);
    // handle input_city_area click event
    lv_keyboard_set_textarea(keyboard, input_city_area); // set the keyboard's input target
    lv_obj_add_event_cb(input_city_area, input_city_area_events, LV_EVENT_ALL, keyboard);
    lv_obj_add_event_cb(keyboard, keyboard_event, LV_EVENT_ALL, NULL);

    printf("UI Screen1 initialized.\n");
}