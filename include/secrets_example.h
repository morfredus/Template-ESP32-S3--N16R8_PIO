#pragma once

#include <stddef.h>

struct WifiCredential {
    const char* ssid;
    const char* password;
};

static const WifiCredential WIFI_CREDENTIALS[] = {
    { "MonSSID1", "MonPass1" },
    { "MonSSID2", "MonPass2" }
};

static const size_t WIFI_CREDENTIALS_COUNT = sizeof(WIFI_CREDENTIALS) / sizeof(WIFI_CREDENTIALS[0]);
