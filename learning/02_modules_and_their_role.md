# Modules and Their Detailed Role

## 🎨 Module: NeoPixelStatus

### 📍 Location
```
src/modules/neopixel_status/
├── neopixel_status.h
└── neopixel_status.cpp
```

### 🎯 Role

Control the **built-in NeoPixel LED** of ESP32-S3 to visually communicate system status.

### 📖 Declaration (neopixel_status.h)

```cpp
enum class StatusColor {
    Idle,        // Idle (white)
    Scanning,    // WiFi scan in progress (yellow)
    Connecting,  // Connecting (blue)
    Connected,   // Connected (green)
    ErrorWifi,   // WiFi error (red)
    ErrorApp     // Application error (purple)
};
```

**What is `enum class`?**
- An enumeration: list of possible values
- Here, we list all possible LED states
- `class` means we must use `StatusColor::Idle` (not just `Idle`)

```cpp
class NeoPixelStatus {
public:
    void begin();                  // Initialize LED
    void set(StatusColor status);  // Change color

private:
    Adafruit_NeoPixel pixel = ...;  // Object that controls the LED
};
```

### 🔧 Implementation (neopixel_status.cpp)

```cpp
void NeoPixelStatus::begin() {
    pixel.begin();
    pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
    set(StatusColor::Idle);
}
```

**Explanation**:
1. Initialize NeoPixel
2. Set brightness (to not blind you!)
3. Set LED to "idle" mode by default

```cpp
void NeoPixelStatus::set(StatusColor status) {
    // Set RGB color based on status
    switch (status) {
        case StatusColor::Idle:       pixel.setPixelColor(0, 255, 255, 255); break; // White
        case StatusColor::Scanning:   pixel.setPixelColor(0, 255, 255, 0);   break; // Yellow
        case StatusColor::Connecting: pixel.setPixelColor(0, 0, 0, 255);     break; // Blue
        case StatusColor::Connected:  pixel.setPixelColor(0, 0, 255, 0);     break; // Green
        case StatusColor::ErrorWifi:  pixel.setPixelColor(0, 255, 0, 0);     break; // Red
        case StatusColor::ErrorApp:   pixel.setPixelColor(0, 128, 0, 128);   break; // Purple
    }
    pixel.show();  // Apply the change
}
```

**Explanation**:
- `switch`: like a series of `if/else` to test `status`
- `setPixelColor(index, R, G, B)`: set color (0-255 for each channel)
- `pixel.show()`: **important!** Without this, nothing displays

### 💡 Why This Module?

Without this module, you'd have to write this code everywhere you want to change the LED:
```cpp
pixel.setPixelColor(0, 255, 0, 0);
pixel.show();
```

With the module:
```cpp
pixel.set(StatusColor::ErrorWifi);  // Clear and simple!
```

**Advantages**:
- ✅ Readable code: `StatusColor::Connected` is clearer than `0, 255, 0`
- ✅ Centralized: to change colors, only one file to modify
- ✅ Reusable: copy this module to another project → it works

---

## 🖥️ Module: OledDisplay

### 📍 Location
```
src/modules/oled_display/
├── oled_display.h
└── oled_display.cpp
```

### 🎯 Role

Control the **SSD1306 OLED screen** (128x64 pixels) to display local information.

### 📖 Declaration (oled_display.h)

```cpp
class OledDisplay {
public:
    bool begin();                    // Initialize screen
    void splash();                   // Startup screen
    void wifiProgress(float progress); // Connection progress (0.0 → 1.0)
    void mainScreen();               // Main screen with IP

private:
    Adafruit_SSD1306 display = ...;  // Screen control object
};
```

### 🔧 Key Implementation Methods

#### begin() - Initialization

```cpp
bool OledDisplay::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;  // Failure
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    return true;  // Success
}
```

**Explanation**:
- `0x3C`: I2C address of the screen (like a postal address on I2C bus)
- `clearDisplay()`: clear everything
- `setTextColor(WHITE)`: white text (monochrome screen)
- `return false/true`: indicates if initialization succeeded

#### splash() - Startup Screen

```cpp
void OledDisplay::splash() {
    display.clearDisplay();
    
    display.setTextSize(2);
    display.setCursor(10, 15);
    display.print(ProjectInfo::NAME.data());
    
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.print("v");
    display.print(ProjectInfo::VERSION.data());
    
    display.display();  // Show the content
}
```

**Explanation**:
- `setTextSize(2)`: text 2x bigger
- `setCursor(x, y)`: text position (upper left corner)
- `print()`: write text (like `Serial.print()`)
- `display()`: **crucial!** Sends content to physical screen

**Analogy**: It's like writing on a draft (buffer), then copying to a clean sheet (`display()`).

#### wifiProgress() - Progress Bar

```cpp
void OledDisplay::wifiProgress(float progress) {
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print("Connexion WiFi...");
    
    // Draw a progress bar
    int barWidth = 100;
    int filledWidth = (int)(progress * barWidth);
    
    display.drawRect(14, 30, barWidth + 2, 12, SSD1306_WHITE);
    display.fillRect(15, 31, filledWidth, 10, SSD1306_WHITE);
    
    display.display();
}
```

**Explanation**:
- `progress`: value between 0.0 (0%) and 1.0 (100%)
- `drawRect()`: draw empty rectangle (bar outline)
- `fillRect()`: draw filled rectangle (the progress)
- `filledWidth`: calculates width to fill based on percentage

**Example**: `progress = 0.5` → `filledWidth = 50 pixels` → bar half filled

---

## 💾 Module: PsramInfo

### 📍 Location
```
src/modules/psram_info/
├── psram_info.h
└── psram_info.cpp
```

### 🎯 Role

Retrieve **PSRAM information** (external RAM memory) of ESP32-S3.

**What is PSRAM?**
- ESP32-S3 has limited internal RAM (~512 KB)
- PSRAM is added external RAM (8 MB on your board)
- Useful for storing large data (images, buffers, etc.)

### 📖 Declaration (psram_info.h)

```cpp
struct PsramInfo {
    bool enabled;        // PSRAM enabled?
    size_t totalBytes;   // Total size in bytes
    size_t freeBytes;    // Free memory in bytes
    std::string type;    // PSRAM type (ex: "OPI")
    std::string mode;    // Mode (ex: "8-line")
    std::string speed;   // Speed (ex: "80MHz")
};

PsramInfo getPsramInfo();
```

**What is `struct`?**
- Like a `class` but simpler
- Groups related data together
- Here: all PSRAM info in one structure

### 🔧 Implementation

```cpp
PsramInfo getPsramInfo() {
    PsramInfo info;
    
    info.enabled = psramFound();  // PSRAM detected?
    
    if (info.enabled) {
        info.totalBytes = ESP.getPsramSize();
        info.freeBytes = ESP.getFreePsram();
        
        // Determine type based on size
        if (info.totalBytes >= 8 * 1024 * 1024) {
            info.type = "OPI (8MB)";
        } else {
            info.type = "QPI (4MB)";
        }
        
        info.mode = "8-line";
        info.speed = "80MHz";
    }
    
    return info;
}
```

---

## 📡 Manager: WifiManager

### 📍 Location
```
src/managers/wifi_manager/
├── wifi_manager.h
└── wifi_manager.cpp
```

### 🎯 Role

**Manager** (orchestrator) that **coordinates** WiFi connection using multiple modules.

**Why "Manager" and not "Module"?**
- A **module** controls a hardware element
- A **manager** coordinates multiple modules for a complex task

### 📖 Declaration (wifi_manager.h)

```cpp
class WifiManager {
public:
    WifiManager(NeoPixelStatus& px, OledDisplay& oled);
    bool connect();  // Connect to WiFi

private:
    NeoPixelStatus& pixel;
    OledDisplay& display;
};
```

**What is `&`?**
- A **reference**: like an alias or pointer to an existing object
- No copy, we directly use the original object
- Safer and simpler than pointers

### 🔧 Key Implementation

```cpp
bool WifiManager::connect() {
    pixel.set(StatusColor::Scanning);
    WiFi.mode(WIFI_STA);  // Station mode (client)
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long start = millis();  // Start time
    unsigned long timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
    
    while (WiFi.status() != WL_CONNECTED) {
        // While not connected
        
        if (millis() - start > timeout) {
            // Timeout exceeded
            pixel.set(StatusColor::ErrorWifi);
            return false;  // Failure
        }
        
        // Calculate progress
        float progress = (float)(millis() - start) / timeout;
        display.wifiProgress(progress);
        
        pixel.set(StatusColor::Connecting);
        delay(Config::WIFI_RETRY_DELAY_MS);
    }
    
    // Connected!
    pixel.set(StatusColor::Connected);
    
    // Save state
    ConfigState::instance().setIp(WiFi.localIP().toString().c_str());
    ConfigState::instance().setSsid(WIFI_SSID);
    
    return true;  // Success
}
```

---

## 📊 Summary

| Component | Type | Responsibility | Dependencies |
|-----------|------|----------------|--------------|
| **NeoPixelStatus** | Module | Control LED | board_config.h |
| **OledDisplay** | Module | Control OLED screen | board_config.h, ConfigState |
| **PsramInfo** | Module | Read PSRAM info | ESP32 SDK |
| **WifiManager** | Manager | Orchestrate WiFi connection | NeoPixel, OLED, Config |

**Golden rule**:
- A **module** = a hardware component or autonomous function
- A **manager** = orchestrator that uses multiple modules

---

**Next step**: Read `03_concepts_cpp_en.md` to understand C++ concepts used (classes, references, namespaces, etc.).
