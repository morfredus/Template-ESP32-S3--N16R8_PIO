#include "wifi_manager.h"
#include "utils/config_state.h"
#include "utils/logging.h"

WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& oled)
    : pixel(px), display(oled) {}

bool WifiManager::connect() {
    LOG_INFO("wifi", "Activation du mode station (STA)");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true, true);
    delay(100);

    LOG_INFO("wifi", "Recherche de réseaux connus");
    pixel.set(StatusColor::Scanning);

    unsigned long start = millis();
    size_t currentIndex = 0;

    while (millis() - start < Config::WIFI_CONNECT_TIMEOUT_MS) {
        const WifiCredential& cred = WIFI_CREDENTIALS[currentIndex];

        LOG_INFO("wifi", "Tentative de connexion au SSID=%s", cred.ssid);
        WiFi.begin(cred.ssid, cred.password);
        pixel.set(StatusColor::Connecting);

        unsigned long attemptStart = millis();
        while (WiFi.status() != WL_CONNECTED &&
               millis() - attemptStart < Config::WIFI_RETRY_DELAY_MS) {

            float globalProgress =
                float(millis() - start) / float(Config::WIFI_CONNECT_TIMEOUT_MS);
            display.wifiProgress(globalProgress);
            delay(100);
        }

        if (WiFi.status() == WL_CONNECTED) {
            IPAddress ip = WiFi.localIP();
            char ipBuffer[16];
            snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u",
                     ip[0], ip[1], ip[2], ip[3]);

            ConfigState::instance().setSsid(cred.ssid);
            ConfigState::instance().setIp(ipBuffer);
            pixel.set(StatusColor::Connected);
            LOG_INFO("wifi", "Connexion établie au SSID=%s", cred.ssid);
            LOG_INFO("wifi", "Adresse IP: %u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
            return true;
        }

        LOG_WARNING("wifi", "Échec de connexion au SSID=%s", cred.ssid);
        currentIndex = (currentIndex + 1) % WIFI_CREDENTIALS_COUNT;
    }

    LOG_ERROR("wifi", "Délai de connexion dépassé (%lu ms)",
              Config::WIFI_CONNECT_TIMEOUT_MS);
    pixel.set(StatusColor::ErrorWifi);
    return false;
}
