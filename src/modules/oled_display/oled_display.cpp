#include "oled_display.h"

#include <Wire.h>
#include <string>

#include "utils/config_state.h"
#include "utils/project_info.h"

bool OledDisplay::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;
    }
    display.clearDisplay();
    display.display();
    return true;
}

void OledDisplay::splash() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 8);
    display.println("Template ESP32-S3");

    display.setCursor(0, 24);
    display.println("Demarrage...");

    display.display();
}

void OledDisplay::wifiProgress(float progress) {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Zone haute (souvent jaune sur bi-color)
    display.setCursor(0, 0);
    display.println(ProjectInfo::NAME.data());
    display.setCursor(0, 8);
    display.print("v");
    display.println(ProjectInfo::VERSION.data());

    // Barre de progression
    int barX = 0;
    int barY = 24;
    int barW = OLED_WIDTH;
    int barH = 10;

    display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);
    int fillW = static_cast<int>(barW * progress);
    if (fillW > 2) {
        display.fillRect(barX + 1, barY + 1, fillW - 2, barH - 2, SSD1306_WHITE);
    }

    display.setCursor(0, 40);
    display.println("Connexion WiFi...");

    display.display();
}

void OledDisplay::mainScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);

    constexpr size_t CHAR_WIDTH = 6; // police par défaut Adafruit
    const size_t maxChars = OLED_WIDTH / CHAR_WIDTH;
    auto truncateText = [maxChars](const std::string& text) {
        if (text.size() > maxChars) {
            return text.substr(0, maxChars);
        }
        return text;
    };

    auto centerText = [this, maxChars](int y, const std::string& text) {
        const size_t len = text.size();
        const size_t width = (len > maxChars ? maxChars : len) * CHAR_WIDTH;
        int x = 0;
        if (OLED_WIDTH > width) {
            x = static_cast<int>((OLED_WIDTH - width) / 2);
        }
        display.setCursor(x, y);
        display.println(text.c_str());
    };

    // Zone haute : nom + version
    display.setCursor(0, 0);
    display.println(ProjectInfo::NAME.data());
    display.setCursor(0, 8);
    display.print("v");
    display.println(ProjectInfo::VERSION.data());

    // Zone basse : SSID + IP (centrés)
    const std::string ssid = truncateText(ConfigState::instance().ssid());
    const std::string ipLine = truncateText(std::string("IP: ") +
                                           ConfigState::instance().ip());

    centerText(24, "SSID:");
    centerText(34, ssid);
    centerText(46, ipLine);

    display.display();
}
