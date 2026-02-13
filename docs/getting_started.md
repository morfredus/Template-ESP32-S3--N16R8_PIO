# Getting Started (Beginner)

Minimum valid version: **1.0.2**

> 📚 **Want to learn in depth?** Check out the comprehensive [educational documentation](/learning/README.md) that explains the code line by line!

## 1) Prerequisites
- VS Code
- PlatformIO extension
- USB cable compatible with data transfer
- ESP32-S3 board and correct drivers

## 2) Configure Wi-Fi credentials
- Open `include/secrets.h`.
- Fill your Wi-Fi credentials in the existing structure.
- Keep the current format unchanged.

## 3) Build and upload firmware
- Build: `pio run`
- Upload firmware: `pio run --target upload`

## 4) Build and upload LittleFS assets
- Build filesystem image: `pio run --target buildfs`
- Upload filesystem image: `pio run --target uploadfs`

## 5) Open serial monitor
- Command: `pio device monitor -b 115200`

## 6) Verify expected behavior
- NeoPixel indicates status (scan/connect/error/success)
- OLED shows splash, progress, then Wi-Fi information
- Web interface loads from LittleFS
- Runtime JSON endpoint is available at `/api/info`
