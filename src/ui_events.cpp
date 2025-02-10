// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include <WiFi.h>
#include "./util/preferences.h"

void scanWifiNetworks(lv_event_t * e)
{
	int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        String networksFound = "";

        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            networksFound = networksFound + "\n" + WiFi.SSID(i).c_str();
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
        }

        lv_dropdown_set_options(ui_WifiNetworksDropdown, networksFound.c_str());

    }
}

void connectToWifi(lv_event_t * e)
{
	char network_ssid[64];
    lv_dropdown_get_selected_str(ui_WifiNetworksDropdown, network_ssid, sizeof(network_ssid));
    
    const char *password_value = lv_textarea_get_text(ui_PasswordInput);
    
	// Save network ssid and password to preferences
	Serial.print(network_ssid);
	Serial.print(password_value);

	prefs.begin("bonsai-vitals", false);
	prefs.putString("ssid", network_ssid);
	prefs.putString("password", password_value);
	prefs.end();

	WiFi.begin(network_ssid, password_value);
}

void resetSettings(lv_event_t * e)
{
	prefs.begin("bonsai-vitals", false);
	prefs.clear();
	prefs.end();
	ESP.restart();
}
