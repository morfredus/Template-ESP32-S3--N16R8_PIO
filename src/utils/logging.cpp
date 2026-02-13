#include "utils/logging.h"

#include <Arduino.h>
#include <cstdarg>
#include <cstdio>

static int serialVprintf(const char* format, va_list args) {
    char buffer[256];
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    if (len <= 0) {
        return len;
    }

    size_t toWrite = static_cast<size_t>(len);
    if (toWrite >= sizeof(buffer)) {
        toWrite = sizeof(buffer) - 1;
    }

    Serial.write(reinterpret_cast<const uint8_t*>(buffer), toWrite);
    return len;
}

void initLogging() {
    Serial.setDebugOutput(true);
    esp_log_set_vprintf(serialVprintf);
    esp_log_level_set("*", ESP_LOG_DEBUG);
}
