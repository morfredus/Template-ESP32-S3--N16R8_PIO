# Detailed Analysis of main.cpp

## 📄 Overview

The `main.cpp` file is the **program entry point**. This is where everything starts and where all modules are orchestrated.

**General structure**:
1. Includes (imports)
2. Global variables
3. Utility functions
4. `setup()` - Initialization
5. `loop()` - Main loop

---

## 1️⃣ Section: Includes

### System Libraries

```cpp
#include <Arduino.h>
```

**Role**: Basic Arduino framework (Serial, delay, millis, etc.)

**Without this line**, you couldn't use:
- `Serial.begin()`, `Serial.print()`
- `delay()`, `millis()`
- Basic Arduino types

---

```cpp
#include <Wire.h>
```

**Role**: Library for **I2C** communication (Inter-Integrated Circuit)

**What is I2C?** A communication bus that allows connecting multiple peripherals (OLED screen, sensors, etc.) using only 2 wires:
- **SDA**: Serial Data (data)
- **SCL**: Serial Clock (clock)

**In this project**: Used for the OLED screen.

---

```cpp
#include <LittleFS.h>
```

**Role**: Filesystem to access files stored in ESP32's flash memory.

**What is LittleFS?**
- A lightweight filesystem for microcontrollers
- Stores files (HTML, CSS, JS) in flash
- Like a mini built-in hard drive

**In this project**: Serves web files (`index.html`, `style.css`) from flash.

---

```cpp
#include <ESPAsyncWebServer.h>
```

**Role**: Library to create an **asynchronous web server**.

**What is "asynchronous"?**
- Server handles HTTP requests in the background
- No need to manage connections in `loop()`
- More efficient and faster than a synchronous server

**In this project**: Serves the web page and JSON API.

---

### Local Project Files

```cpp
#include "board_config.h"
#include "managers/wifi_manager/wifi_manager.h"
#include "modules/oled_display/oled_display.h"
#include "modules/neopixel_status/neopixel_status.h"
#include "modules/psram_info/psram_info.h"
#include "utils/config_state.h"
#include "utils/project_info.h"
```

**Role**: Include classes and definitions from project modules.

---

## 2️⃣ Section: Global Variables

```cpp
AsyncWebServer server(WEB_SERVER_PORT);
```

**Explanation**:
- Creates a `server` object of type `AsyncWebServer`
- `WEB_SERVER_PORT`: constant defined in `board_config.h` (80 = standard HTTP port)
- **Global** variable: accessible throughout the file

**Why global?** The server must live for the entire duration of the program and be accessible in multiple functions.

---

```cpp
NeoPixelStatus pixel;
OledDisplay oled;
```

**Explanation**:
- Creates two global objects: `pixel` and `oled`
- Instances of `NeoPixelStatus` and `OledDisplay` classes

**Why global?**
- Used in `setup()` for initialization
- Passed by reference to `WifiManager`
- Must persist for the entire program duration

---

## 3️⃣ Function: buildInfoJson()

```cpp
static String buildInfoJson() {
```

**Role**: Builds a JSON string containing system information.

**`static`**: The function is **private** to the file, not accessible from other files.

---

### JSON Buffer Initialization

```cpp
String json;
json.reserve(1024);
```

**Explanation**:
- Creates an empty string `json`
- `reserve(1024)`: **preallocates** 1024 bytes of memory

**Why `reserve()`?**
- ✅ Optimization: avoids reallocating memory multiple times
- ✅ Performance: faster concatenation
- ✅ Stability: reduces memory fragmentation

**Analogy**: Like reserving a large blank sheet instead of adding sheets as you go.

---

### Building the JSON

```cpp
json += "{";

json += "\"project\":{";
json += "\"name\":\"";    json += ProjectInfo::NAME.data();    json += "\",";
json += "\"version\":\""; json += ProjectInfo::VERSION.data(); json += "\"},";

json += "\"wifi\":{";
json += "\"ssid\":\""; json += ConfigState::instance().ssid().c_str(); json += "\",";
json += "\"ip\":\"";   json += ConfigState::instance().ip().c_str();   json += "\"},";

// ... PSRAM and board info

json += "}";
return json;
```

**Result example**:
```json
{
  "project": {
    "name": "Template-ESP32-S3",
    "version": "1.0.0"
  },
  "wifi": {
    "ssid": "MyWiFi",
    "ip": "192.168.1.42"
  },
  "psram": {
    "enabled": true,
    "total": 8388608,
    "free": 7234567
  },
  "board": {
    "flash_bytes": 16777216,
    "heap_free": 245632
  }
}
```

---

## 4️⃣ Function: setup()

### 🎬 Serial Initialization

```cpp
Serial.begin(115200);
delay(200);
```

**Explanation**:
- `Serial.begin(115200)`: Initialize serial communication at 115200 bauds
- `delay(200)`: Wait 200 ms to stabilize serial connection

**Why 115200?** Common baud rate for ESP32, fast enough for debugging.

---

### 💡 NeoPixel Initialization

```cpp
pixel.begin();
```

**Explanation**:
- Calls the `begin()` method of the `pixel` object
- Initializes the NeoPixel LED and sets it to "Idle" mode (white)

---

### 🖥️ OLED Screen Initialization

```cpp
Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
```

**Explanation**:
- Initializes the I2C bus with pins defined in `board_config.h`
- `OLED_SDA_PIN = 15`: data pin
- `OLED_SCL_PIN = 16`: clock pin

**Why before `oled.begin()`?** The I2C bus must be initialized before communicating with the screen.

---

```cpp
if (!oled.begin()) {
    pixel.set(StatusColor::ErrorApp);
    while (true) delay(1000);
}
```

**Explanation**:
- Attempts to initialize the OLED screen
- `!oled.begin()`: if initialization fails (returns `false`)
- Changes LED to **purple** (application error)
- **Infinite loop**: the program stops here

**Why an infinite loop?** Without a screen, impossible to continue. Better to stop cleanly than to crash.

---

### 🎨 Startup Screen (Splash)

```cpp
oled.splash();
delay(2000);
```

**Explanation**:
- Displays the startup screen (project name + version)
- Waits 2000 ms (2 seconds) for the user to see it

---

### 📡 WiFi Connection

```cpp
WifiManager wifi(pixel, oled);
bool ok = wifi.connect();

if (!ok) {
    return;
}
```

**Explanation**:
1. Creates a `wifi` object of type `WifiManager`
2. Passes references `pixel` and `oled` to the constructor
3. Calls `connect()` to connect to WiFi
4. If failure (`ok == false`), **exits `setup()`**

**What happens on failure?**
- `return;` exits `setup()`
- Program moves to `loop()`
- Since `loop()` is empty, nothing happens → red LED stays lit

---

### 📊 Main Screen Display

```cpp
oled.mainScreen();
```

**Explanation**:
- Displays the main screen with SSID and IP
- Called only if WiFi connection succeeded

---

### 📦 Filesystem Initialization

```cpp
if (!LittleFS.begin(true)) {
    pixel.set(StatusColor::ErrorApp);
    return;
}
```

**Explanation**:
- `LittleFS.begin(true)`: mounts the filesystem
  - `true`: format if necessary (first use)
- If failure, LED purple (app error) and exit

**Why?** Without filesystem, impossible to serve web pages.

---

### 🌐 Web Server Configuration

```cpp
server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
```

**Explanation**:
- `serveStatic("/", LittleFS, "/")`: serve files from root folder `/` of LittleFS
- `.setDefaultFile("index.html")`: if accessing `/`, serve `index.html`

**What happens?**
- Request `http://192.168.1.42/` → responds with `index.html`
- Request `http://192.168.1.42/style.css` → responds with `style.css`

---

```cpp
server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", buildInfoJson());
});
```

**Explanation**:
- `server.on()`: defines an **endpoint** (access point)
- `"/api/info"`: endpoint URL
- `HTTP_GET`: HTTP GET method
- `[](...) { ... }`: **lambda** (anonymous function)

**What is a lambda?**
- A function without a name, defined directly
- Here, it's called when accessing `/api/info`

**What does the lambda do?**
- `request->send(200, "application/json", buildInfoJson())`
  - Code 200: HTTP OK
  - MIME type: `application/json`
  - Content: JSON built by `buildInfoJson()`

---

```cpp
server.begin();
```

**Explanation**:
- Starts the web server
- From this moment, server listens on port 80

---

## 5️⃣ Function: loop()

```cpp
void loop() {
    // AsyncWebServer → empty loop
}
```

**Explanation**:
- The `loop()` function is **empty**!
- This is normal: the asynchronous web server handles everything in the background

**How does it work?**
- `ESPAsyncWebServer` uses interrupts and FreeRTOS tasks
- HTTP requests are handled automatically
- No code needed in `loop()`

**Analogy**: Like an Apache web server running in the background, no need to monitor it.

---

## 📊 Complete Execution Flow

```
┌─────────────────────────────┐
│  ESP32 Startup              │
└──────────┬──────────────────┘
           ▼
┌─────────────────────────────┐
│  setup()                    │
│  ├─ Serial.begin()          │ ← Initialize serial communication
│  ├─ pixel.begin()           │ ← Initialize LED
│  ├─ Wire.begin()            │ ← Initialize I2C bus
│  ├─ oled.begin()            │ ← Initialize OLED screen
│  ├─ oled.splash()           │ ← Display startup screen
│  │   └─ delay(2000)         │ ← Wait 2 seconds
│  ├─ wifi.connect()          │ ← Connect to WiFi
│  │   ├─ Network scan        │
│  │   ├─ Connection          │
│  │   └─ Save IP             │
│  ├─ oled.mainScreen()       │ ← Display main screen
│  ├─ LittleFS.begin()        │ ← Mount filesystem
│  ├─ server.serveStatic()    │ ← Configure server (files)
│  ├─ server.on("/api/info")  │ ← Configure JSON API
│  └─ server.begin()          │ ← Start web server
└──────────┬──────────────────┘
           ▼
┌─────────────────────────────┐
│  loop()                     │ ← Empty loop (async server)
│  (empty)                    │
└──────────┬──────────────────┘
           │
           └─────────► Infinite repetition
```

---

**Next step**: Read `05_configuration_en.md` to understand configuration files (GPIO, timeouts, secrets).
