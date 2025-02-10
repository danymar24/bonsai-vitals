// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_WiFiScreen_screen_init(void)
{
    ui_WiFiScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_WiFiScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_WiFiScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_WiFiScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsScreenBackButton = lv_imgbtn_create(ui_WiFiScreen);
    lv_imgbtn_set_src(ui_SettingsScreenBackButton, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_786474294, NULL);
    lv_obj_set_height(ui_SettingsScreenBackButton, 64);
    lv_obj_set_width(ui_SettingsScreenBackButton, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_SettingsScreenBackButton, -327);
    lv_obj_set_y(ui_SettingsScreenBackButton, -171);
    lv_obj_set_align(ui_SettingsScreenBackButton, LV_ALIGN_CENTER);

    ui_ScanWifiNetworksButton = lv_btn_create(ui_WiFiScreen);
    lv_obj_set_width(ui_ScanWifiNetworksButton, 282);
    lv_obj_set_height(ui_ScanWifiNetworksButton, 50);
    lv_obj_set_x(ui_ScanWifiNetworksButton, 235);
    lv_obj_set_y(ui_ScanWifiNetworksButton, -186);
    lv_obj_set_align(ui_ScanWifiNetworksButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ScanWifiNetworksButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ScanWifiNetworksButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ScanWifiNetworksButtonLabel = lv_label_create(ui_ScanWifiNetworksButton);
    lv_obj_set_width(ui_ScanWifiNetworksButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ScanWifiNetworksButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ScanWifiNetworksButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ScanWifiNetworksButtonLabel, "Scan Networks");
    lv_obj_set_style_text_font(ui_ScanWifiNetworksButtonLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WifiNetworksDropdown = lv_dropdown_create(ui_WiFiScreen);
    lv_dropdown_set_options(ui_WifiNetworksDropdown, "");
    lv_obj_set_width(ui_WifiNetworksDropdown, 385);
    lv_obj_set_height(ui_WifiNetworksDropdown, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_WifiNetworksDropdown, -1);
    lv_obj_set_y(ui_WifiNetworksDropdown, -80);
    lv_obj_set_align(ui_WifiNetworksDropdown, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_WifiNetworksDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_font(ui_WifiNetworksDropdown, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(lv_dropdown_get_list(ui_WifiNetworksDropdown), &lv_font_montserrat_24,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PasswordInput = lv_textarea_create(ui_WiFiScreen);
    lv_obj_set_width(ui_PasswordInput, 385);
    lv_obj_set_height(ui_PasswordInput, 51);
    lv_obj_set_x(ui_PasswordInput, -1);
    lv_obj_set_y(ui_PasswordInput, -19);
    lv_obj_set_align(ui_PasswordInput, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_PasswordInput, "Password");
    lv_textarea_set_password_mode(ui_PasswordInput, true);
    lv_obj_set_style_text_font(ui_PasswordInput, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_ConnectToNetworkButton = lv_btn_create(ui_WiFiScreen);
    lv_obj_set_width(ui_ConnectToNetworkButton, 179);
    lv_obj_set_height(ui_ConnectToNetworkButton, 50);
    lv_obj_set_x(ui_ConnectToNetworkButton, -2);
    lv_obj_set_y(ui_ConnectToNetworkButton, 51);
    lv_obj_set_align(ui_ConnectToNetworkButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ConnectToNetworkButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_ConnectToNetworkButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ConnectToNetworkButtonLabel = lv_label_create(ui_ConnectToNetworkButton);
    lv_obj_set_width(ui_ConnectToNetworkButtonLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ConnectToNetworkButtonLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ConnectToNetworkButtonLabel, -1);
    lv_obj_set_y(ui_ConnectToNetworkButtonLabel, -1);
    lv_obj_set_align(ui_ConnectToNetworkButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_ConnectToNetworkButtonLabel, "Connect");
    lv_obj_set_style_text_font(ui_ConnectToNetworkButtonLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PasswordKeyboard = lv_keyboard_create(ui_WiFiScreen);
    lv_obj_set_width(ui_PasswordKeyboard, 796);
    lv_obj_set_height(ui_PasswordKeyboard, -18);
    lv_obj_set_x(ui_PasswordKeyboard, 1);
    lv_obj_set_y(ui_PasswordKeyboard, 124);
    lv_obj_set_align(ui_PasswordKeyboard, LV_ALIGN_CENTER);

    lv_obj_set_style_text_font(ui_PasswordKeyboard, &lv_font_montserrat_32, LV_PART_ITEMS | LV_STATE_DEFAULT);

    ui_ConnectedNetworkLabel = lv_label_create(ui_WiFiScreen);
    lv_obj_set_width(ui_ConnectedNetworkLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ConnectedNetworkLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ConnectedNetworkLabel, 207);
    lv_obj_set_y(ui_ConnectedNetworkLabel, -124);
    lv_obj_set_align(ui_ConnectedNetworkLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_ConnectedNetworkLabel, "");
    lv_obj_set_style_text_color(ui_ConnectedNetworkLabel, lv_color_hex(0xE7E7E7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ConnectedNetworkLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_ConnectedNetworkLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_SettingsScreenBackButton, ui_event_SettingsScreenBackButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScanWifiNetworksButton, ui_event_ScanWifiNetworksButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PasswordInput, ui_event_PasswordInput, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ConnectToNetworkButton, ui_event_ConnectToNetworkButton, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(ui_PasswordKeyboard, ui_PasswordInput);
    uic_WiFiScreen = ui_WiFiScreen;
    uic_SettingsScreenBackButton = ui_SettingsScreenBackButton;
    uic_ScanWifiNetworksButton = ui_ScanWifiNetworksButton;
    uic_ScanWifiNetworksButtonLabel = ui_ScanWifiNetworksButtonLabel;
    uic_WifiNetworksDropdown = ui_WifiNetworksDropdown;
    uic_PasswordInput = ui_PasswordInput;
    uic_ConnectToNetworkButton = ui_ConnectToNetworkButton;
    uic_ConnectToNetworkButtonLabel = ui_ConnectToNetworkButtonLabel;
    uic_PasswordKeyboard = ui_PasswordKeyboard;
    uic_ConnectedNetworkLabel = ui_ConnectedNetworkLabel;

}
