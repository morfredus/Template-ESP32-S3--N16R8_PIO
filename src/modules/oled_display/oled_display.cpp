#include "oled_display.h"

#include <Wire.h>

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

    // Zone haute : nom + version
    display.setCursor(0, 0);
    display.println(ProjectInfo::NAME.data());
    display.setCursor(0, 8);
    display.println(ProjectInfo::VERSION.data());

    // Zone basse : SSID + IP
    display.setCursor(0, 24);
    display.print("SSID: ");
    display.println(ConfigState::instance().ssid().c_str());

    display.setCursor(0, 36);
    display.print("IP: ");
    display.println(ConfigState::instance().ip().c_str());

    display.display();
}
