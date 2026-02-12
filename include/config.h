#pragma once
#include <stdint.h>

namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000; // 20 s total
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;   // 0,5 s par tentative
    constexpr uint32_t SPLASH_SCREEN_MS        = 2000;  // splash OLED
}
