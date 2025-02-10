#include "../ui.h"
#include <Arduino.h>
#include <WiFi.h>

static void getWifiStatus()
{
    switch(WiFi.status()){
        case WL_NO_SSID_AVAIL:
        case WL_CONNECT_FAILED:
            lv_label_set_text(ui_ConnectedNetworkLabel, "Incorrect password.");
            lv_obj_set_style_text_color(ui_ConnectedNetworkLabel, lv_color_hex(0x90000), LV_PART_MAIN | LV_STATE_DEFAULT);
            return;
        case WL_CONNECTED:
            String connectedWifi = "Connected to: " + WiFi.SSID() + " (IP: " + WiFi.localIP().toString() + ")";
            lv_label_set_text(ui_ConnectedNetworkLabel, connectedWifi.c_str());
            lv_obj_clear_flag(ui_MoistureScreenWifiIndicator, LV_OBJ_FLAG_HIDDEN);      /// Flags

            return;
    }

}