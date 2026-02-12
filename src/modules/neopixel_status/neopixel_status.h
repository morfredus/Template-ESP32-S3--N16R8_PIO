#pragma once

#include <Adafruit_NeoPixel.h>

#include "board_config.h"

enum class StatusColor {
    Idle,
    Scanning,
    Connecting,
    Connected,
    ErrorWifi,
    ErrorApp
};

class NeoPixelStatus {
public:
    void begin();
    void set(StatusColor status);

private:
    Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
};
