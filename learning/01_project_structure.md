# ESP32 Project Structure

## 📂 Overview

```
Template-ESP32-S3--N16R8_PIO/
│
├── 📁 src/                    ← MAIN SOURCE CODE
│   ├── main.cpp               ← Program entry point
│   ├── managers/              ← High-level managers
│   ├── modules/               ← Hardware modules (screen, LED, etc.)
│   └── utils/                 ← Utilities and helpers
│
├── 📁 include/                ← CONFIGURATION FILES
│   ├── board_config.h         ← GPIO pin configuration
│   ├── config.h               ← General configuration
│   └── secrets.h              ← WiFi credentials (DO NOT SHARE)
│
├── 📁 data/                   ← WEB FILES (HTML, CSS, JS)
│   ├── index.html             ← Web page served by ESP32
│   └── style.css              ← Page styles
│
├── 📁 docs/                   ← USER DOCUMENTATION
│   └── (guides, manuals)
│
├── 📁 learning/               ← EDUCATIONAL DOCUMENTATION
│   └── (detailed C++ guides)
│
├── 📁 lib/                    ← LOCAL LIBRARIES (if needed)
│
├── 📁 scripts/                ← PYTHON UTILITY SCRIPTS
│   └── auto_fs.py             ← Automatic filesystem upload
│
├── 📁 test/                   ← UNIT TESTS
│
├── platformio.ini             ← PlatformIO configuration
└── README.md                  ← Project documentation
```

---

## 🎯 The `src/` Folder (Source Code)

This is **the heart of the program**. All C++ code is here.

### 📄 main.cpp

**Role**: Program entry point, orchestrates all modules.

**Contains**:
- `setup()`: Initialization function (executed once)
- `loop()`: Main loop function (in this project, it's empty)

**Analogy**: It's the conductor who makes all musicians play at the right time.

---

### 📁 managers/ (Managers)

**Managers** are responsible for **coordinating multiple modules** to accomplish a complex task.

#### wifi_manager/

```
wifi_manager/
├── wifi_manager.h      ← WifiManager class declaration
└── wifi_manager.cpp    ← Implementation
```

**Role**: Manage WiFi connection.

**Why a manager?** Connecting to WiFi involves:
- Interacting with the pixel (display status)
- Interacting with the OLED screen (display progress)
- Managing connection attempts
- Saving state in ConfigState

It's an **orchestrator** that uses multiple modules.

---

### 📁 modules/ (Hardware Modules)

**Modules** are **autonomous** components that control a specific hardware element.

#### neopixel_status/

```
neopixel_status/
├── neopixel_status.h      ← Declaration
└── neopixel_status.cpp    ← Implementation
```

**Role**: Control the built-in NeoPixel LED to display system status.

**Features**:
- `begin()`: Initialize LED
- `set(StatusColor)`: Change color based on status

**Why a module?** The LED is a hardware component that can be reused in other projects.

#### oled_display/

```
oled_display/
├── oled_display.h      ← Declaration
└── oled_display.cpp    ← Implementation
```

**Role**: Control the SSD1306 OLED screen (128x64).

**Features**:
- `begin()`: Initialize screen
- `splash()`: Display startup screen
- `wifiProgress(float)`: Display WiFi connection progress
- `mainScreen()`: Display main screen with IP

**Why a module?** The OLED screen is a reusable hardware component.

#### psram_info/

```
psram_info/
├── psram_info.h      ← Declaration
└── psram_info.cpp    ← Implementation
```

**Role**: Retrieve information about ESP32-S3's PSRAM memory.

**Features**:
- `getPsramInfo()`: Returns a structure with all PSRAM info

**Why a module?** It's an autonomous feature that can be used anywhere.

---

### 📁 utils/ (Utilities)

**Utilities** are files that provide **constants**, **types**, or **helper functions** used throughout the project.

#### config_constants.h

**Role**: Defines configuration constants from compiled values.

**Content**:
```cpp
namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME    = PROJECT_NAME;
    constexpr std::string_view PROJECT_VERSION = PROJECT_VERSION;
}
```

**Why?** The values `PROJECT_NAME` and `PROJECT_VERSION` are defined in `platformio.ini` and injected at compile time. This file makes them easily accessible.

#### config_state.h

**Role**: Store runtime configuration state.

**Content**:
- Assigned IP address
- Connected WiFi SSID

**Pattern used**: **Singleton** (single instance accessible everywhere)

**Why?** Multiple parts of the program need to know the IP and SSID. Instead of passing them as parameters everywhere, we use a global state.

#### project_info.h

**Role**: Provide access to project name and version.

**Why a separate file?** Centralization: if you want to display the version, you include this file. It's simple and clear.

---

## 🔧 The `include/` Folder (Configuration)

### board_config.h

**Role**: Defines **all GPIO pins** and hardware parameters.

**Content**:
```cpp
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
#define NEOPIXEL_PIN  48
```

**Why separate?** If you change boards or wiring, you only modify **this file**. The rest of the code doesn't need to change.

**IMPORTANT RULE**: Never modify this file without a valid reason!

### config.h

**Role**: Software configuration (timeouts, delays, etc.).

**Content**:
```cpp
namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;
}
```

**Why separate?** All behavior parameters are centralized. Easy to adjust without searching through all the code.

### secrets.h

**Role**: Contains **sensitive credentials** (WiFi SSID, password).

**WARNING**: This file must **NEVER** be shared or pushed to GitHub!

---

## 📦 The `data/` Folder (Filesystem)

**Role**: Contains web files (HTML, CSS, JS) that will be **uploaded** to the ESP32's flash memory (LittleFS filesystem).

**How it works**:
1. Files are uploaded with `auto_fs.py` script or via PlatformIO
2. ESP32 reads them from its flash memory
3. Web server serves them to HTTP clients

**Why?** ESP32 doesn't have a hard drive. We store files in part of its flash memory.

---

## ⚙️ platformio.ini

**Role**: PlatformIO configuration file (development environment).

**Important content**:
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

build_flags = 
    -D PROJECT_NAME='"Template-ESP32-S3"'
    -D PROJECT_VERSION='"1.0.0"'
```

- **build_flags**: Defines constants available in C++ code
- Required libraries are listed here

**Why here?** Centralizing version and name makes them easy to change for all project files.

---

## 🔗 How Everything is Connected

```
main.cpp
  │
  ├─ Include board_config.h ──────────► GPIO definitions
  ├─ Include config.h ────────────────► Timeouts, parameters
  │
  ├─ Create NeoPixelStatus pixel ─────► LED module
  ├─ Create OledDisplay oled ─────────► Screen module
  │
  ├─ Create WifiManager
  │    └─ Uses pixel + oled ──────────► Manager = orchestrator
  │
  ├─ Call getPsramInfo() ─────────────► PSRAM info module
  │
  └─ Use ConfigState::instance() ─────► Global state
```

---

## 🎓 Summary: Why This Organization?

| Type | Role | Examples |
|------|------|----------|
| **Modules** | Control a hardware element | LED, OLED screen, PSRAM |
| **Managers** | Orchestrate multiple modules | WiFi (uses LED + screen) |
| **Utils** | Provide reusable tools | Constants, global state |
| **Include** | Hardware/software configuration | GPIO, timeouts, secrets |

**Advantages**:
- ✅ Each file has a clear responsibility
- ✅ Easy to test and debug
- ✅ Reusable in other projects
- ✅ Modifying one module doesn't affect others

---

**Next step**: Read `02_modules_et_leur_role_en.md` to understand each module in detail.
