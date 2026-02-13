# Introduction to C++ and this ESP32 Project

## 📘 What is C++?

**C++** is a compiled programming language, meaning your code is transformed into machine instructions before being executed. This is different from interpreted languages like Python.

### Why C++ for ESP32?

1. **Performance**: C++ is very fast because it communicates directly with hardware
2. **Control**: You precisely manage memory and resources
3. **Arduino Framework**: A simplified layer that makes C++ more accessible for microcontrollers

## 🎯 What This Project Does

This project is a **template** (model) to quickly start with an ESP32-S3. It includes:

- ✅ Automatic WiFi connection
- ✅ Web server to display information
- ✅ OLED screen for local display
- ✅ NeoPixel LED for status indication
- ✅ PSRAM memory management

## 📁 Basic Concepts to Know

### 1. `.h` Files (Headers)

`.h` files are like **summaries** or **tables of contents**. They declare what exists (functions, classes, constants) but generally don't contain the complete implementation.

**Why?** This allows other files to know what's available without needing all the code.

### 2. `.cpp` Files (Source)

`.cpp` files contain the **actual implementation** of the code, i.e., how things work.

### 3. The include (#include)

```cpp
#include "wifi_manager.h"
```

This line says: "I'm going to use things defined in wifi_manager.h, go fetch this file and integrate it here."

### 4. Classes

A **class** is a template for creating objects. It's like a construction blueprint.

```cpp
class WifiManager {
    // Declaration: what a WifiManager can do
};
```

### 5. Namespaces

A **namespace** is like a folder to organize code and avoid name conflicts.

```cpp
namespace Config {
    constexpr uint32_t WIFI_TIMEOUT = 20000;
}
// To use it: Config::WIFI_TIMEOUT
```

## 🔄 ESP32 Program Lifecycle

```
┌─────────────────┐
│   Startup       │
│   (power on)    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   setup()       │ ← Executed ONCE at startup
│                 │   (WiFi initialization, screen, etc.)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   loop()        │ ← Executed in an INFINITE LOOP
│                 │   (event handling, updates)
└────────┬────────┘
         │
         └──────────► Return to beginning of loop()
```

**Important**: In this project, `loop()` is empty because we use an asynchronous web server that handles requests automatically!

## 🧩 Code Organization

### Modular Approach

Instead of putting all code in one big file, we **split it into modules**:

- Each module has a clear responsibility
- Modules are reusable
- Code is easier to understand and maintain

**Analogy**: It's like building a house with separate rooms (kitchen, bedroom, living room) rather than one big open space without walls.

## 📚 Guide Structure

1. **00_introduction_cpp_en.md** ← You are here
2. **01_structure_du_projet_en.md** - Folder organization
3. **02_modules_et_leur_role_en.md** - Detail of each module
4. **03_concepts_cpp_en.md** - C++ concepts used
5. **04_main_explique_en.md** - Line-by-line analysis of main program
6. **05_configuration_en.md** - Configuration files

---

**Next step**: Read `01_structure_du_projet_en.md` to understand how the project is organized.
