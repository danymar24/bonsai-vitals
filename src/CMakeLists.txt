SET(SOURCES screens/ui_MoistureScreen.c
    screens/ui_SettingsScreen.c
    screens/ui_WiFiScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_gauge_png.c
    images/ui_img_1588936248.c
    images/ui_img_cogwheel_png.c
    images/ui_img_wifi_png.c
    images/ui_img_786474294.c)

add_library(ui ${SOURCES})
