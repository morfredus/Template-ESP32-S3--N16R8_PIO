#include "psram_info.h"

#include <esp_heap_caps.h>

PsramInfo getPsramInfo() {
    PsramInfo info;

    info.enabled = psramFound();
    info.totalBytes = ESP.getPsramSize();
    info.freeBytes  = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

    info.type = info.enabled ? "External PSRAM" : "None";

    if (info.enabled) {
        if (info.totalBytes >= 8 * 1024 * 1024) {
            info.mode = "OPI (Octal)";
            info.speed = "120 MHz";
        } else {
            info.mode = "SPI (Quad)";
            info.speed = "40 MHz";
        }
    } else {
        info.mode = "N/A";
        info.speed = "N/A";
    }

    return info;
}
