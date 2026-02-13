# Project Structure

Minimum valid version: **1.0.7**

> 📚 **Want detailed explanations?** See the [educational documentation](/learning/README.md) for a deep dive into project architecture!

## Directory tree (current)
- `src/main.cpp`: application entry point
- `src/modules/`
  - `neopixel_status/`
  - `oled_display/`
  - `psram_info/`
- `src/managers/`
  - `wifi_manager/`
- `src/utils/`
  - shared config/state/project headers
  - logging helpers (`logging.h`, `logging.cpp`)
- `include/`
  - protected configuration files only
- `data/`
  - static web files for LittleFS
- `docs/`
  - user documentation (EN/FR)

## Naming and placement rules
- Classes: PascalCase
- Functions: camelCase
- Variables: snake_case
- Constants/macros: UPPER_SNAKE_CASE
- Module files: `module_name.h` and `module_name.cpp`
- No project headers in `include/` except protected files

## Protected files in include/
- `board_config.h`
- `secrets.h`
- `secrets_example.h`
- `config.h`
- `README`
