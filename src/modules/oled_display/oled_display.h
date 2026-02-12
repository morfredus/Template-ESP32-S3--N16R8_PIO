#pragma once

#include <Adafruit_SSD1306.h>

#include "board_config.h"

class OledDisplay {
public:
    bool begin();
    void splash();
    void wifiProgress(float progress); // 0.0 → 1.0
    void mainScreen();

private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
};
