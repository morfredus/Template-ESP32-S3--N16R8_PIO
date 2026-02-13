# C++ Concepts Used in This Project

## 📚 Introduction

This document explains the C++ concepts you encounter in this project. Each concept is explained simply, with analogies and examples.

---

## 1️⃣ Classes and Objects

### 🤔 What is a Class?

A **class** is a **blueprint** that describes:
- **Data** (member variables)
- **Behaviors** (member functions = methods)

**Analogy**: A class is like a house construction plan.

### 🏠 Example: The NeoPixelStatus Class

```cpp
class NeoPixelStatus {
public:
    void begin();
    void set(StatusColor status);

private:
    Adafruit_NeoPixel pixel;
};
```

**Breakdown**:
- `class NeoPixelStatus`: Defines a new class named `NeoPixelStatus`
- `public:`: Methods accessible from outside
- `private:`: Data accessible only inside the class

### 🔨 Creating an Object (Instance)

```cpp
NeoPixelStatus pixel;  // Create an object of type NeoPixelStatus
```

**Analogy**: The class is the blueprint, the `pixel` object is the house built from the blueprint.

### 📞 Using an Object

```cpp
pixel.begin();                      // Call the begin() method
pixel.set(StatusColor::Connected);  // Call the set() method
```

**The dot `.`** means "access a member of the object".

---

## 2️⃣ References and Pointers

### 🔗 References (`&`)

A **reference** is an **alias** (nickname) for an existing object.

```cpp
void WifiManager(NeoPixelStatus& px) {
    // px is a reference, not a copy
    px.set(StatusColor::Scanning);  // Modifies the original object
}
```

**Why use `&`?**
- ✅ Avoids copying the object (saves memory)
- ✅ Modifies the original object (not a copy)

**Analogy**: Instead of photocopying a document, we point to the original document.

### 🆚 With or Without Reference

```cpp
// WITHOUT reference (copy)
void function1(NeoPixelStatus px) {
    px.set(...);  // Modifies the COPY, not the original
}

// WITH reference (alias)
void function2(NeoPixelStatus& px) {
    px.set(...);  // Modifies the ORIGINAL
}
```

**In this project**: We always use references to avoid expensive copies.

---

## 3️⃣ Namespaces

### 🗂️ What is a Namespace?

A **namespace** is a **logical folder** to organize code and avoid name conflicts.

**Problem without namespace**:
```cpp
// File A
int TIMEOUT = 1000;

// File B
int TIMEOUT = 5000;  // ERROR: conflict!
```

**Solution with namespace**:
```cpp
// File A
namespace ModuleA {
    int TIMEOUT = 1000;
}

// File B
namespace ModuleB {
    int TIMEOUT = 5000;
}

// Usage
int x = ModuleA::TIMEOUT;  // 1000
int y = ModuleB::TIMEOUT;  // 5000
```

### 📦 Example in This Project

```cpp
namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;
}

// Usage
int timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
```

**`::` (scope resolution operator)**: "Access something in the `Config` namespace"

---

## 4️⃣ Constants and constexpr

### 📌 `const`: Immutable Value

```cpp
const int MAX_RETRIES = 10;
MAX_RETRIES = 20;  // ERROR: cannot modify a const
```

**Purpose**: Protects against accidental modifications.

### ⚡ `constexpr`: Compile-Time Constant

```cpp
constexpr uint32_t TIMEOUT = 20000;
```

**Difference with `const`**:
- `const`: immutable value (can be calculated at runtime)
- `constexpr`: value **calculated at compile time** (faster, less memory)

**In this project**: We use `constexpr` for all fixed configurations.

---

## 5️⃣ Enum and Enum Class

### 🎨 `enum`: Enumeration

An **enumeration** defines a set of **named values**.

```cpp
enum Color {
    Red,
    Green,
    Blue
};

Color c = Red;
```

**Problem**: Names pollute the global space.

### 🎯 `enum class`: Safe Enumeration

```cpp
enum class StatusColor {
    Idle,
    Scanning,
    Connecting,
    Connected
};

StatusColor state = StatusColor::Idle;  // Must prefix with StatusColor::
```

**Advantages**:
- ✅ No namespace pollution
- ✅ Safer (no implicit conversion to int)
- ✅ More readable

**In this project**: We always use `enum class`.

---

## 6️⃣ Structures (struct)

### 📦 What is a Structure?

A **structure** groups multiple related variables into a single entity.

```cpp
struct PsramInfo {
    bool enabled;
    size_t totalBytes;
    size_t freeBytes;
    std::string type;
    std::string mode;
    std::string speed;
};
```

**Usage**:
```cpp
PsramInfo info;
info.enabled = true;
info.totalBytes = 8388608;
```

### 🆚 struct vs class

| Aspect | `struct` | `class` |
|--------|----------|---------|
| **Default visibility** | `public` | `private` |
| **Typical usage** | Group data | Encapsulate logic + data |

---

## 7️⃣ std::string vs String

### 🔤 Two String Types

#### Arduino `String` (discouraged)

```cpp
String s = "Hello";
s += " World";
```

**Problems**:
- ❌ Heap memory fragmentation
- ❌ Slow and unpredictable
- ❌ Can cause crashes on ESP32

#### C++ `std::string` (recommended)

```cpp
std::string s = "Hello";
s += " World";
```

**Advantages**:
- ✅ Standard C++ (portable)
- ✅ More efficient memory management
- ✅ Safer

**In this project**: We **always** use `std::string`.

---

## 8️⃣ std::string_view

### 👀 Non-Owning View

`std::string_view` is a **view** on a string, without copying.

```cpp
constexpr std::string_view PROJECT_NAME = "My Project";
```

**Advantages**:
- ✅ No copy (saves memory)
- ✅ Can be `constexpr` (evaluated at compile time)
- ✅ Very fast

**In this project**: Used for `PROJECT_NAME` and `PROJECT_VERSION` because they're compile-time constants.

---

## 9️⃣ Singleton Pattern

### 🏛️ Single Global Instance

The **Singleton** guarantees that only **one instance** of a class exists.

**Example: ConfigState**

```cpp
class ConfigState {
public:
    static ConfigState& instance() {
        static ConfigState inst;  // Created ONLY ONCE
        return inst;
    }

    void setIp(const std::string& ip) { ipAddress = ip; }
    const std::string& ip() const { return ipAddress; }

private:
    ConfigState() = default;  // Private constructor

    std::string ipAddress;
    std::string connectedSsid;
};
```

**Usage**:
```cpp
ConfigState::instance().setIp("192.168.1.100");
std::string ip = ConfigState::instance().ip();
```

**How it works**:

1. **`static ConfigState& instance()`**: Static function (no object needed to call it)
2. **`static ConfigState inst;`**: Static local variable (created once, lives until program end)
3. **`ConfigState() = default;`**: Default constructor, but **private** → impossible to create an object directly

**Why?**
- ✅ Global state accessible everywhere
- ✅ Single instance guaranteed
- ✅ Lazy initialization

---

## 🔟 Includes and Pragma Once

### 📥 `#include`: File Inclusion

```cpp
#include <Arduino.h>       // System library (angle brackets <>)
#include "wifi_manager.h"  // Local file (quotes "")
```

**How it works?**
The preprocessor **copy-pastes** the included file content at this location.

### 🛡️ `#pragma once`: Protection Against Double Inclusion

**Problem**:
```cpp
// file A.h
struct Data { int x; };

// file B.h
#include "A.h"

// file C.cpp
#include "A.h"
#include "B.h"  // A.h included twice → ERROR: redefinition of Data
```

**Solution**:
```cpp
// A.h
#pragma once

struct Data { int x; };
```

**Effect**: File A.h will only be included **once** even if requested multiple times.

**In this project**: All `.h` files start with `#pragma once`.

---

## 1️⃣1️⃣ Constructors and Initialization Lists

### 🏗️ Constructor

A **constructor** is a special function called when **creating** an object.

```cpp
class WifiManager {
public:
    WifiManager(NeoPixelStatus& px, OledDisplay& ol);
};
```

### 📋 Initialization List

```cpp
WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& ol)
    : pixel(px), display(ol) {
    // Constructor body (can be empty)
}
```

**Explanation**:
- `: pixel(px), display(ol)`: **initialization list**
- **Before** the brace `{`, we initialize members
- More efficient than assignment in the body

**Why initialization list?**
- ✅ Required for references (you can't assign a reference)
- ✅ More efficient (direct initialization)
- ✅ Clear initialization order

---

## 1️⃣2️⃣ Public, Private, Protected

### 🔓 Access Levels

```cpp
class Example {
public:
    void publicMethod();    // Accessible everywhere

private:
    void privateMethod();   // Accessible only within the class
    int privateData;

protected:
    void protectedMethod(); // Accessible in class and derived classes
};
```

**Golden rule**:
- Data → **private**
- Interface methods → **public**
- Implementation methods → **private**

**Why?** **Encapsulation**: hide internal details, expose only the interface.

---

## 📊 Summary of Concepts

| Concept | Usage in Project |
|---------|------------------|
| **Classes** | NeoPixelStatus, OledDisplay, WifiManager |
| **References (`&`)** | Pass objects without copying |
| **Namespaces** | Config, ProjectInfo, ConfigConstants |
| **constexpr** | Configuration constants |
| **enum class** | StatusColor (LED states) |
| **struct** | PsramInfo (group data) |
| **std::string** | Character strings (IP, SSID) |
| **Singleton** | ConfigState (unique global state) |
| **#pragma once** | Header protection |
| **#define** | GPIO macros |

---

**Next step**: Read `04_main_explique_en.md` for a detailed line-by-line analysis of the main.cpp file.
