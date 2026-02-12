# User Guide

Minimum valid version: **1.0.2**

## Main features
- Multi-credential Wi-Fi connection attempts
- OLED display feedback for startup and connectivity
- NeoPixel status indicator
- Embedded web UI served from LittleFS
- Device information API at `/api/info`

## Web endpoint details
- `GET /api/info`
- Returns JSON with:
  - project name and version
  - connected SSID and local IP
  - PSRAM details (enabled/size/free/type/mode/speed)
  - board runtime details (flash size/free heap/chip revision)

## Safe update workflow
1. Update code and data files.
2. Run `pio run`.
3. Run `pio run --target buildfs`.
4. Upload firmware, then filesystem.
5. Validate serial output and web endpoint.

## Troubleshooting
- If upload works but web content is old, rebuild/upload LittleFS.
- If Wi-Fi fails, verify credentials and network availability.
- If OLED stays blank, verify board wiring/pins in `include/board_config.h`.
