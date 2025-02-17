// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_SettingsScreen_screen_init(void)
{
    ui_SettingsScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SettingsScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SettingsScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsBackButton = lv_imgbtn_create(ui_SettingsScreen);
    lv_imgbtn_set_src(ui_SettingsBackButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_786474294, NULL);
    lv_imgbtn_set_src(ui_SettingsBackButton, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_786474294, NULL);
    lv_obj_set_height(ui_SettingsBackButton, 64);
    lv_obj_set_width(ui_SettingsBackButton, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_SettingsBackButton, -353);
    lv_obj_set_y(ui_SettingsBackButton, -192);
    lv_obj_set_align(ui_SettingsBackButton, LV_ALIGN_CENTER);

    ui_WiFiSettingsButton = lv_btn_create(ui_SettingsScreen);
    lv_obj_set_width(ui_WiFiSettingsButton, 396);
    lv_obj_set_height(ui_WiFiSettingsButton, 97);
    lv_obj_set_x(ui_WiFiSettingsButton, 2);
    lv_obj_set_y(ui_WiFiSettingsButton, -108);
    lv_obj_set_align(ui_WiFiSettingsButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_WiFiSettingsButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_WiFiSettingsButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_WiFiSettingsButtonLabel = lv_label_create(ui_WiFiSettingsButton);
    lv_obj_set_width(ui_WiFiSettingsButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_WiFiSettingsButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_WiFiSettingsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WiFiSettingsButtonLabel, "WiFi");
    lv_obj_set_style_text_font(ui_WiFiSettingsButtonLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ResetToFactoryButton = lv_btn_create(ui_SettingsScreen);
    lv_obj_set_width(ui_ResetToFactoryButton, 396);
    lv_obj_set_height(ui_ResetToFactoryButton, 97);
    lv_obj_set_x(ui_ResetToFactoryButton, 3);
    lv_obj_set_y(ui_ResetToFactoryButton, 117);
    lv_obj_set_align(ui_ResetToFactoryButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ResetToFactoryButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ResetToFactoryButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ResetToFactoryButtonLabel = lv_label_create(ui_ResetToFactoryButton);
    lv_obj_set_width(ui_ResetToFactoryButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ResetToFactoryButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ResetToFactoryButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ResetToFactoryButtonLabel, "Reset settings");
    lv_obj_set_style_text_font(ui_ResetToFactoryButtonLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ResetToFactoryConfirmButton = lv_btn_create(ui_SettingsScreen);
    lv_obj_set_width(ui_ResetToFactoryConfirmButton, 396);
    lv_obj_set_height(ui_ResetToFactoryConfirmButton, 97);
    lv_obj_set_x(ui_ResetToFactoryConfirmButton, 3);
    lv_obj_set_y(ui_ResetToFactoryConfirmButton, 117);
    lv_obj_set_align(ui_ResetToFactoryConfirmButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ResetToFactoryConfirmButton, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ResetToFactoryConfirmButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ResetToFactoryConfirmButton, lv_color_hex(0x870000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ResetToFactoryConfirmButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ResetToFactoryConfirmButtonLabel = lv_label_create(ui_ResetToFactoryConfirmButton);
    lv_obj_set_width(ui_ResetToFactoryConfirmButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ResetToFactoryConfirmButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ResetToFactoryConfirmButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ResetToFactoryConfirmButtonLabel, "Confirm Reset");
    lv_obj_set_style_text_font(ui_ResetToFactoryConfirmButtonLabel, &lv_font_montserrat_40,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TimeSettingsButton = lv_btn_create(ui_SettingsScreen);
    lv_obj_set_width(ui_TimeSettingsButton, 396);
    lv_obj_set_height(ui_TimeSettingsButton, 97);
    lv_obj_set_x(ui_TimeSettingsButton, 3);
    lv_obj_set_y(ui_TimeSettingsButton, 4);
    lv_obj_set_align(ui_TimeSettingsButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_TimeSettingsButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_TimeSettingsButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_TimeSettingsButtonLabel = lv_label_create(ui_TimeSettingsButton);
    lv_obj_set_width(ui_TimeSettingsButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_TimeSettingsButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_TimeSettingsButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TimeSettingsButtonLabel, "Time");
    lv_obj_set_style_text_font(ui_TimeSettingsButtonLabel, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_SettingsBackButton, ui_event_SettingsBackButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WiFiSettingsButton, ui_event_WiFiSettingsButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ResetToFactoryButton, ui_event_ResetToFactoryButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ResetToFactoryConfirmButton, ui_event_ResetToFactoryConfirmButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TimeSettingsButton, ui_event_TimeSettingsButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsScreen, ui_event_SettingsScreen, LV_EVENT_ALL, NULL);
    uic_SettingsScreen = ui_SettingsScreen;
    uic_SettingsBackButton = ui_SettingsBackButton;
    uic_WiFiSettingsButton = ui_WiFiSettingsButton;
    uic_WiFiSettingsButtonLabel = ui_WiFiSettingsButtonLabel;
    uic_ResetToFactoryButton = ui_ResetToFactoryButton;
    uic_ResetToFactoryButtonLabel = ui_ResetToFactoryButtonLabel;
    uic_ResetToFactoryConfirmButton = ui_ResetToFactoryConfirmButton;
    uic_ResetToFactoryConfirmButtonLabel = ui_ResetToFactoryConfirmButtonLabel;
    uic_TimeSettingsButton = ui_TimeSettingsButton;
    uic_TimeSettingsButtonLabel = ui_TimeSettingsButtonLabel;

}
