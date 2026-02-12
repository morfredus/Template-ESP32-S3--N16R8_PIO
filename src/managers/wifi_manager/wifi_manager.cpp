#include "wifi_manager.h"
#include "utils/config_state.h"

WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& oled)
    : pixel(px), display(oled) {}

bool WifiManager::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true, true);
    delay(100);

    pixel.set(StatusColor::Scanning);

    unsigned long start = millis();
    size_t currentIndex = 0;

    while (millis() - start < Config::WIFI_CONNECT_TIMEOUT_MS) {
        const WifiCredential& cred = WIFI_CREDENTIALS[currentIndex];

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
            ConfigState::instance().setSsid(cred.ssid);
            ConfigState::instance().setIp(WiFi.localIP().toString().c_str());
            pixel.set(StatusColor::Connected);
            return true;
        }

        currentIndex = (currentIndex + 1) % WIFI_CREDENTIALS_COUNT;
    }

    pixel.set(StatusColor::ErrorWifi);
    return false;
}
