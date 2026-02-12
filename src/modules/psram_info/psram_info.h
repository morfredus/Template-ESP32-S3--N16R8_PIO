#pragma once

#include <Arduino.h>

struct PsramInfo {
    bool enabled;
    size_t totalBytes;
    size_t freeBytes;
    const char* type;
    const char* mode;
    const char* speed;
};

PsramInfo getPsramInfo();
