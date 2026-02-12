#include "neopixel_status.h"

void NeoPixelStatus::begin() {
    pixel.begin();
    pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
    set(StatusColor::Idle);
}

void NeoPixelStatus::set(StatusColor status) {
    uint32_t color = 0;

    switch (status) {
        case StatusColor::Idle:        color = pixel.Color(0, 0, 0); break;
        case StatusColor::Scanning:    color = pixel.Color(0, 0, 255); break;   // bleu
        case StatusColor::Connecting:  color = pixel.Color(255, 165, 0); break; // orange
        case StatusColor::Connected:   color = pixel.Color(0, 255, 0); break;   // vert
        case StatusColor::ErrorWifi:   color = pixel.Color(255, 0, 0); break;   // rouge
        case StatusColor::ErrorApp:    color = pixel.Color(255, 0, 255); break; // magenta
    }

    pixel.setPixelColor(0, color);
    pixel.show();
}
