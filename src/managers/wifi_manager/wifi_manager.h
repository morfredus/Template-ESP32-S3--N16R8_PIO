#pragma once

#include <WiFi.h>

#include "secrets.h"
#include "modules/neopixel_status/neopixel_status.h"
#include "modules/oled_display/oled_display.h"
#include "config.h"

class WifiManager {
public:
    WifiManager(NeoPixelStatus& px, OledDisplay& oled);

    // Retourne true si connecté
    bool connect();

private:
    NeoPixelStatus& pixel;
    OledDisplay& display;
};
