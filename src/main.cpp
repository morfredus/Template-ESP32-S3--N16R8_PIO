#include <Arduino.h>
#include <Wire.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

#include "board_config.h"
#include "managers/wifi_manager/wifi_manager.h"
#include "modules/oled_display/oled_display.h"
#include "modules/neopixel_status/neopixel_status.h"
#include "modules/psram_info/psram_info.h"
#include "utils/config_state.h"
#include "utils/logging.h"
#include "utils/project_info.h"

AsyncWebServer server(WEB_SERVER_PORT);

NeoPixelStatus pixel;
OledDisplay oled;

static String buildInfoJson() {
    String json;
    json.reserve(1024);

    PsramInfo ps = getPsramInfo();

    json += "{";

    json += "\"project\":{";
    json += "\"name\":\"";    json += ProjectInfo::NAME.data();    json += "\",";
    json += "\"version\":\""; json += ProjectInfo::VERSION.data(); json += "\"},";
    
    json += "\"wifi\":{";
    json += "\"ssid\":\""; json += ConfigState::instance().ssid().c_str(); json += "\",";
    json += "\"ip\":\"";   json += ConfigState::instance().ip().c_str();   json += "\"},";

    json += "\"psram\":{";
    json += "\"enabled\":"; json += ps.enabled ? "true" : "false"; json += ",";
    json += "\"total\":";   json += String(ps.totalBytes); json += ",";
    json += "\"free\":";    json += String(ps.freeBytes);  json += ",";
    json += "\"type\":\"";  json += ps.type;               json += "\",";
    json += "\"mode\":\"";  json += ps.mode;               json += "\",";
    json += "\"speed\":\""; json += ps.speed;              json += "\"";
    json += "},";

    json += "\"board\":{";
    json += "\"flash_bytes\":"; json += String(ESP.getFlashChipSize()); json += ",";
    json += "\"heap_free\":";   json += String(ESP.getFreeHeap());      json += ",";
    json += "\"chip_revision\":"; json += String(ESP.getChipRevision());
    json += "}";

    json += "}";
    return json;
}

void setup() {
    Serial.begin(115200);
    delay(200);

    initLogging();
    LOG_INFO("main", "Démarrage du système");

    LOG_DEBUG("main", "Initialisation NeoPixel");
    pixel.begin();

    LOG_DEBUG("main", "Initialisation OLED");
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
    if (!oled.begin()) {
        LOG_ERROR("main", "Échec de l'initialisation OLED");
        pixel.set(StatusColor::ErrorApp);
        while (true) delay(1000);
    }

    oled.splash();
    delay(2000);

    LOG_INFO("wifi", "Connexion Wi-Fi...");
    WifiManager wifi(pixel, oled);
    bool ok = wifi.connect();

    if (!ok) {
        LOG_ERROR("wifi", "Impossible de se connecter au Wi-Fi");
        return;
    }

    LOG_INFO("wifi", "Connecté au SSID=%s", ConfigState::instance().ssid().c_str());

    oled.mainScreen();

    if (!LittleFS.begin(true)) {
        LOG_ERROR("fs", "Échec du montage LittleFS");
        pixel.set(StatusColor::ErrorApp);
        return;
    }

    LOG_INFO("fs", "LittleFS monté avec succès");

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest* request) {
        LOG_DEBUG("http", "GET /api/info");
        request->send(200, "application/json", buildInfoJson());
    });

    server.begin();

    LOG_INFO("http", "Serveur HTTP prêt sur http://%s:%u",
             ConfigState::instance().ip().c_str(), WEB_SERVER_PORT);
}

void loop() {
    // AsyncWebServer → loop vide
}
