# ESP32-S3 Template - Educational Project for Beginners

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange.svg)](https://platformio.org/)
[![Beginner Friendly](https://img.shields.io/badge/Beginner-Friendly-green.svg)](learning/README.md)
[![Documentation](https://img.shields.io/badge/Documentation-Complete-blue.svg)](learning/README.md)
[![C++](https://img.shields.io/badge/C%2B%2B-Explained-red.svg)](learning/03_concepts_cpp.md)

> 🎓 **Complete template with line-by-line educational documentation**  
> Perfect for learning C++, PlatformIO, and ESP32 embedded development

Minimum valid version: **1.0.2**

## 🚀 What is this?

A **ready-to-use** PlatformIO template for ESP32-S3 with **comprehensive beginner documentation**. Ideal for:
- 📚 **Learning C++** and embedded development
- 🎯 **Understanding** how a professional ESP32 project works
- 🛠️ **Quick-starting** your own IoT projects

## ✨ Included Features
- Wi-Fi connection using multiple credentials from `include/secrets.h`
- OLED display status screens
- Onboard NeoPixel status feedback
- LittleFS static web server (`data/index.html`, `data/style.css`)
- Runtime info API at `/api/info`

## Quick start (beginner)
1. Install VS Code, PlatformIO extension, and USB drivers for your ESP32-S3 board.
2. Copy `include/secrets_example.h` to your local secure setup if needed and keep `include/secrets.h` aligned with your credentials format.
3. Connect the board via USB.
4. Build firmware: `pio run`
5. Upload firmware: `pio run --target upload`
6. Build filesystem image: `pio run --target buildfs`
7. Upload filesystem image: `pio run --target uploadfs`
8. Open serial monitor: `pio device monitor -b 115200`

## Project layout
- `src/main.cpp`: application entry point
- `src/modules/`: hardware/feature modules
- `src/managers/`: orchestration managers
- `src/utils/`: shared utility/config headers
- `include/`: protected configuration headers only
- `docs/`: user documentation (EN/FR)

## Documentation
- Beginner guide: `docs/getting_started.md`
- User guide: `docs/user_guide.md`
- Architecture and tree: `docs/project_structure.md`
- French versions are available with `_fr.md` suffix.

### 📚 Educational documentation (C++ learning)

For beginners who want to **deeply understand** how the code works and grasp C++ concepts, comprehensive French documentation is available in the `learning/` folder:

- **Complete guide**: [`learning/README.md`](learning/README.md) - Entry point with navigation
- Introduction to C++: [`learning/00_introduction_cpp.md`](learning/00_introduction_cpp.md)
- Project structure: [`learning/01_structure_du_projet.md`](learning/01_structure_du_projet.md)
- Detailed modules: [`learning/02_modules_et_leur_role.md`](learning/02_modules_et_leur_role.md)
- C++ concepts: [`learning/03_concepts_cpp.md`](learning/03_concepts_cpp.md)
- main.cpp analysis: [`learning/04_main_explique.md`](learning/04_main_explique.md)
- Configuration: [`learning/05_configuration.md`](learning/05_configuration.md)

This documentation explains the code **line by line**, technical choices, and concepts used, with analogies and concrete examples.

**Note**: Documentation is currently available in French only.
