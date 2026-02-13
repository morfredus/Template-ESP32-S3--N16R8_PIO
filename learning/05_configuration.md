# Configuration Files

## 📋 Overview

Configuration is **separated** from the main code to facilitate:
- ✅ Modifications without touching the code
- ✅ Adaptation to different boards
- ✅ Security (secrets not shared)

**Configuration files**:
1. `board_config.h` - Hardware configuration (GPIO)
2. `config.h` - Software configuration (timeouts, parameters)
3. `secrets.h` - Sensitive credentials (WiFi)
4. `config_constants.h` - Derived constants (in src/utils)
5. `config_state.h` - Runtime configuration state

---

## 1️⃣ board_config.h (Hardware Configuration)

### 📍 Location
```
include/board_config.h
```

### 🎯 Role

Defines **all GPIO pins** and hardware parameters specific to the board.

### 📄 Complete Content

```cpp
#pragma once

// OLED I2C (recommended by Espressif)
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
#define OLED_WIDTH    128
#define OLED_HEIGHT   64

// Built-in NeoPixel
#define NEOPIXEL_PIN        48
#define NEOPIXEL_BRIGHTNESS 38  // ~15%

// Web server
#define WEB_SERVER_PORT 80
```

---

### 📖 Detailed Explanation

#### OLED I2C

```cpp
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
```

**Role**: Defines GPIO pins for the OLED screen's I2C bus.

**What is I2C?**
- **I**nter-**I**ntegrated **C**ircuit
- 2-wire communication bus:
  - **SDA** (Serial Data): data line
  - **SCL** (Serial Clock): clock line

**Why these pins?**
- GPIO 15 and 16 are recommended by Espressif for I2C on ESP32-S3
- Avoids conflicts with other internal peripherals

**Analogy**: Like a USB cable with 2 wires (data + clock) to connect multiple devices.

---

```cpp
#define OLED_WIDTH    128
#define OLED_HEIGHT   64
```

**Role**: OLED screen dimensions in pixels.

**Why define as constant?**
- ✅ Easy to change if you use another screen (e.g., 128x32)
- ✅ Used to calculate display positions

**Usage in code**:
```cpp
display.begin(OLED_WIDTH, OLED_HEIGHT);
int centerX = OLED_WIDTH / 2;  // Calculate center
```

---

#### NeoPixel

```cpp
#define NEOPIXEL_PIN        48
```

**Role**: GPIO pin connected to the built-in NeoPixel LED.

**Why GPIO 48?**
- On ESP32-S3-DevKitC-1, the built-in RGB LED is wired to GPIO 48
- It's fixed on this board, cannot be changed

---

```cpp
#define NEOPIXEL_BRIGHTNESS 38  // ~15%
```

**Role**: LED brightness (0-255).

**Why 38 (~15%)?**
- NeoPixel LEDs are **very bright** at 100%
- 38/255 ≈ 15%: comfortable for eyes
- Also saves a bit of energy

**How it works?**
```cpp
pixel.setBrightness(NEOPIXEL_BRIGHTNESS);  // Apply brightness
```

---

#### Web Server

```cpp
#define WEB_SERVER_PORT 80
```

**Role**: Network port for the web server.

**Why 80?**
- Standard HTTP port (like normal websites)
- Allows access without specifying port: `http://192.168.1.42/` instead of `http://192.168.1.42:8080/`

**Common ports**:
- 80: HTTP (unsecure)
- 443: HTTPS (secure)
- 8080: Alternative HTTP (often for development)

---

### 🛡️ IMPORTANT RULE

**❌ DO NOT MODIFY this file without a valid reason!**

**Why?**
- GPIOs are tied to **physical hardware**
- Modifying without knowledge → risk of conflict or malfunction
- Other developers rely on these values

**When to modify?**
- Changing ESP32 board
- Adding a new peripheral (with confirmation of free GPIOs)
- Resolving a hardware conflict

---

## 2️⃣ config.h (Software Configuration)

### 📍 Location
```
include/config.h
```

### 🎯 Role

Defines program **behavior parameters** (timeouts, delays, etc.).

### 📄 Complete Content

```cpp
#pragma once
#include <stdint.h>

namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000; // 20 s total
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;   // 0.5 s per attempt
    constexpr uint32_t SPLASH_SCREEN_MS        = 2000;  // OLED splash
}
```

---

### 📖 Detailed Explanation

#### WiFi Connection Timeout

```cpp
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000; // 20 s total
```

**Role**: Maximum duration (in milliseconds) to attempt WiFi connection.

**Breakdown**:
- `constexpr`: constant evaluated at compile time
- `uint32_t`: unsigned 32-bit integer (0 to 4,294,967,295)
- `= 20000`: 20,000 milliseconds = 20 seconds

**Why 20 seconds?**
- Long enough to give WiFi time to connect
- Not too long to avoid blocking indefinitely

**Usage**:
```cpp
unsigned long timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
if (millis() - start > timeout) {
    // Timeout exceeded, give up
}
```

---

#### Delay Between Attempts

```cpp
constexpr uint32_t WIFI_RETRY_DELAY_MS = 500;   // 0.5 s per attempt
```

**Role**: Delay (in milliseconds) between each WiFi status check.

**Why 500 ms?**
- Checks status every 0.5 seconds
- Allows regular screen and LED updates
- Not too fast (CPU savings), not too slow (responsiveness)

**Usage**:
```cpp
while (WiFi.status() != WL_CONNECTED) {
    // ...
    delay(Config::WIFI_RETRY_DELAY_MS);  // Wait before retrying
}
```

---

#### Splash Screen Duration

```cpp
constexpr uint32_t SPLASH_SCREEN_MS = 2000;  // OLED splash
```

**Role**: Display duration (in milliseconds) of the startup screen.

**Why 2 seconds?**
- Long enough to read name and version
- Not too long to avoid slowing down startup

---

### 💡 Advantages of This Approach

**Centralization**:
- All parameters in one place
- Easy to adjust

**Readability**:
```cpp
// ❌ Hard to understand
delay(2000);

// ✅ Clear and explicit
delay(Config::SPLASH_SCREEN_MS);
```

**Maintainability**:
- To change WiFi timeout, modify **one place only**
- No need to search through all code

---

## 3️⃣ secrets.h (Sensitive Credentials)

### 📍 Location
```
include/secrets.h
```

### 🎯 Role

Contains **sensitive credentials** (WiFi SSID, password).

### 📄 Typical Structure

```cpp
#pragma once

// WiFi credentials
#define WIFI_SSID     "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_Password"
```

**⚠️ WARNING**:
- ❌ NEVER share this file
- ❌ NEVER push it to GitHub/GitLab
- 🛡️ It's in `.gitignore` to avoid accidental commits

---

### 🔒 Security

**Why a separate file?**
- ✅ Code can be shared without exposing credentials
- ✅ Each developer has their own credentials
- ✅ Facilitates versioning (secrets not in Git)

**Example file provided**:
```
include/secrets_example.h
```

**Usage**:
1. Copy `secrets_example.h` → `secrets.h`
2. Modify with your real credentials
3. `secrets.h` is ignored by Git

---

## 4️⃣ config_constants.h (Derived Constants)

### 📍 Location
```
src/utils/config_constants.h
```

### 🎯 Role

Makes accessible constants **injected at compile time** (from `platformio.ini`).

### 📄 Complete Content

```cpp
#pragma once
#include <string_view>

namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME    = PROJECT_NAME;
    constexpr std::string_view PROJECT_VERSION = PROJECT_VERSION;
}
```

---

### 📖 Detailed Explanation

#### Origin: platformio.ini

```ini
[env:esp32-s3-devkitc-1]
build_flags = 
    -D PROJECT_NAME='"Template-ESP32-S3"'
    -D PROJECT_VERSION='"1.0.0"'
```

**Explanation**:
- `-D PROJECT_NAME='"..."'`: defines a `PROJECT_NAME` macro with the value in quotes
- These macros are available **everywhere** in C++ code

---

#### Wrapping in a Namespace

```cpp
namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME = PROJECT_NAME;
}
```

**Why this wrapper?**
1. **Organization**: Group in a clear namespace
2. **Type safe**: `std::string_view` instead of raw macro
3. **Readability**: `ConfigConstants::PROJECT_NAME` vs `PROJECT_NAME` (macro)

**What is `std::string_view`?**
- **Non-owning** view on a string
- No copy, just a pointer + size
- Very efficient for constants

---

## 5️⃣ config_state.h (Runtime State)

### 📍 Location
```
src/utils/config_state.h
```

### 🎯 Role

Stores **runtime configuration state** (values that change during the program).

### 📄 Complete Content

```cpp
#pragma once
#include <string>

class ConfigState {
public:
    static ConfigState& instance() {
        static ConfigState inst;
        return inst;
    }

    void setIp(const std::string& ip) { ipAddress = ip; }
    const std::string& ip() const { return ipAddress; }

    void setSsid(const std::string& s) { connectedSsid = s; }
    const std::string& ssid() const { return connectedSsid; }

private:
    ConfigState() = default;

    std::string ipAddress;
    std::string connectedSsid;
};
```

---

### 📖 Detailed Explanation

#### Singleton Pattern

```cpp
static ConfigState& instance() {
    static ConfigState inst;
    return inst;
}
```

**Role**: Guarantees only **one instance** of `ConfigState` exists.

**How it works**:
1. `static ConfigState inst;`: static local variable, created **once**
2. `return inst;`: always returns the same instance

**Usage**:
```cpp
ConfigState::instance().setIp("192.168.1.42");
std::string ip = ConfigState::instance().ip();
```

---

### 💡 Complete Usage

#### 1. Save State (after WiFi connection)

```cpp
// In wifi_manager.cpp
ConfigState::instance().setIp(WiFi.localIP().toString().c_str());
ConfigState::instance().setSsid(WIFI_SSID);
```

#### 2. Read State (in main.cpp)

```cpp
// In buildInfoJson()
json += "\"ssid\":\"";
json += ConfigState::instance().ssid().c_str();
json += "\",";

json += "\"ip\":\"";
json += ConfigState::instance().ip().c_str();
json += "\"";
```

#### 3. Display State (on OLED screen)

```cpp
// In oled_display.cpp
display.print(ConfigState::instance().ssid().c_str());
display.print(ConfigState::instance().ip().c_str());
```

---

### 🆚 config.h vs config_state.h

| File | Type | Values | When |
|------|------|--------|------|
| **config.h** | Constants | Fixed (timeouts, delays) | Compile time |
| **config_state.h** | State | Variables (IP, SSID) | Runtime |

**Analogy**:
- `config.h`: Game rules (fixed)
- `config_state.h`: Current score (changes during game)

---

## 📊 Configuration Files Summary

| File | Location | Role | Modifiable |
|------|----------|------|------------|
| **board_config.h** | `include/` | GPIO, ports | ❌ Rarely |
| **config.h** | `include/` | Timeouts, parameters | ✅ Often |
| **secrets.h** | `include/` | WiFi, passwords | ✅ Per user |
| **config_constants.h** | `src/utils/` | Name, version (from ini) | ❌ Never |
| **config_state.h** | `src/utils/` | IP, SSID (runtime) | ❌ Never (code) |

---

## 🎯 Best Practices

### ✅ TO DO

- Centralize parameters in `config.h`
- Use explicit names (`WIFI_TIMEOUT_MS` rather than `TIMEOUT`)
- Comment non-obvious values
- Use `constexpr` for fixed values

### ❌ TO AVOID

- Hardcode values directly in code
- Modify `board_config.h` without valid reason
- Push `secrets.h` to a public repository
- Use magic values without names (`delay(2000);` → why 2000?)

---

## 🎓 Conclusion

Configuration files allow to:
- ✅ **Separate** configuration from code
- ✅ **Adapt** project easily to different boards
- ✅ **Secure** sensitive credentials
- ✅ **Maintain** code easily

**Golden rule**: When you hesitate between hardcoding a value or putting it in configuration, **put it in configuration**!

---

**End of documentation!** You now have a complete understanding of the project. 🎉
