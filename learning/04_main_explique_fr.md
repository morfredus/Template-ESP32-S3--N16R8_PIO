# Analyse détaillée de main.cpp

## 📄 Vue d'ensemble

Le fichier `main.cpp` est le **point d'entrée** du programme. C'est ici que tout commence et où sont orchestrés tous les modules.

**Structure générale** :
1. Includes (imports)
2. Variables globales
3. Fonctions utilitaires
4. `setup()` - Initialisation
5. `loop()` - Boucle principale

---

## 1️⃣ Section : Includes

### Bibliothèques système

```cpp
#include <Arduino.h>
```

**Rôle** : Framework Arduino de base (Serial, delay, millis, etc.)

**Sans cette ligne**, vous ne pourriez pas utiliser :
- `Serial.begin()`, `Serial.print()`
- `delay()`, `millis()`
- Les types de base Arduino

---

```cpp
#include <Wire.h>
```

**Rôle** : Bibliothèque pour la communication **I2C** (Inter-Integrated Circuit)

**C'est quoi l'I2C ?** Un bus de communication qui permet de connecter plusieurs périphériques (écran OLED, capteurs, etc.) sur seulement 2 fils :
- **SDA** : Serial Data (données)
- **SCL** : Serial Clock (horloge)

**Dans ce projet** : Utilisé pour l'écran OLED.

---

```cpp
#include <LittleFS.h>
```

**Rôle** : Système de fichiers pour accéder aux fichiers stockés dans la mémoire flash de l'ESP32.

**C'est quoi LittleFS ?**
- Un système de fichiers léger pour microcontrôleurs
- Stocke des fichiers (HTML, CSS, JS) dans la flash
- Comme un mini disque dur intégré

**Dans ce projet** : Sert les fichiers web (`index.html`, `style.css`) depuis la flash.

---

```cpp
#include <ESPAsyncWebServer.h>
```

**Rôle** : Bibliothèque pour créer un **serveur web asynchrone**.

**C'est quoi "asynchrone" ?**
- Le serveur gère les requêtes HTTP en arrière-plan
- Pas besoin de gérer les connexions dans `loop()`
- Plus efficace et rapide qu'un serveur synchrone

**Dans ce projet** : Sert la page web et l'API JSON.

---

### Fichiers locaux du projet

```cpp
#include "board_config.h"
```

**Rôle** : Définitions des broches GPIO et paramètres matériels.

**Contenu** :
```cpp
#define OLED_SDA_PIN 15
#define OLED_SCL_PIN 16
#define NEOPIXEL_PIN 48
#define WEB_SERVER_PORT 80
```

---

```cpp
#include "managers/wifi_manager/wifi_manager.h"
```

**Rôle** : Inclut la classe `WifiManager` pour gérer la connexion WiFi.

---

```cpp
#include "modules/oled_display/oled_display.h"
#include "modules/neopixel_status/neopixel_status.h"
#include "modules/psram_info/psram_info.h"
```

**Rôle** : Inclut les classes des modules matériels (écran, LED, PSRAM).

---

```cpp
#include "utils/config_state.h"
#include "utils/project_info.h"
```

**Rôle** : Inclut les utilitaires (état global, infos projet).

---

## 2️⃣ Section : Variables globales

```cpp
AsyncWebServer server(WEB_SERVER_PORT);
```

**Explication** :
- Crée un objet `server` de type `AsyncWebServer`
- `WEB_SERVER_PORT` : constante définie dans `board_config.h` (80 = port HTTP standard)
- Variable **globale** : accessible partout dans le fichier

**Pourquoi global ?** Le serveur doit vivre pendant toute la durée du programme et être accessible dans plusieurs fonctions.

---

```cpp
NeoPixelStatus pixel;
OledDisplay oled;
```

**Explication** :
- Crée deux objets globaux : `pixel` et `oled`
- Instances des classes `NeoPixelStatus` et `OledDisplay`

**Pourquoi global ?**
- Utilisés dans `setup()` pour l'initialisation
- Passés en référence à `WifiManager`
- Doivent persister pendant toute la durée du programme

---

## 3️⃣ Fonction : buildInfoJson()

```cpp
static String buildInfoJson() {
```

**Rôle** : Construit une chaîne JSON contenant les informations du système.

**`static`** : La fonction est **privée** au fichier, elle n'est pas accessible depuis d'autres fichiers.

---

### Initialisation du buffer JSON

```cpp
String json;
json.reserve(1024);
```

**Explication** :
- Crée une chaîne vide `json`
- `reserve(1024)` : **préalloue** 1024 octets de mémoire

**Pourquoi `reserve()` ?**
- ✅ Optimisation : évite de réallouer la mémoire plusieurs fois
- ✅ Performance : concaténation plus rapide
- ✅ Stabilité : réduit la fragmentation mémoire

**Analogie** : Comme réserver une grande feuille blanche plutôt que d'ajouter des feuilles au fur et à mesure.

---

### Récupération des infos PSRAM

```cpp
PsramInfo ps = getPsramInfo();
```

**Explication** :
- Appelle la fonction `getPsramInfo()` du module psram_info
- Retourne une structure `PsramInfo` contenant toutes les infos

---

### Construction du JSON

```cpp
json += "{";

json += "\"project\":{";
json += "\"name\":\"";    json += ProjectInfo::NAME.data();    json += "\",";
json += "\"version\":\""; json += ProjectInfo::VERSION.data(); json += "\"},";
```

**Explication** :
- `+=` : concatène (ajoute à la fin)
- `\"` : guillemet échappé (pour avoir `"` dans la chaîne)
- `ProjectInfo::NAME.data()` : récupère le nom du projet (défini dans platformio.ini)

**Résultat** :
```json
{
  "project": {
    "name": "Template-ESP32-S3",
    "version": "1.0.0"
  },
```

---

```cpp
json += "\"wifi\":{";
json += "\"ssid\":\""; json += ConfigState::instance().ssid().c_str(); json += "\",";
json += "\"ip\":\"";   json += ConfigState::instance().ip().c_str();   json += "\"},";
```

**Explication** :
- `ConfigState::instance()` : récupère l'instance unique du singleton
- `.ssid()` : récupère le SSID WiFi connecté
- `.c_str()` : convertit `std::string` en `const char*` (format C)

**Résultat** :
```json
  "wifi": {
    "ssid": "MonReseauWiFi",
    "ip": "192.168.1.42"
  },
```

---

```cpp
json += "\"psram\":{";
json += "\"enabled\":"; json += ps.enabled ? "true" : "false"; json += ",";
json += "\"total\":";   json += String(ps.totalBytes); json += ",";
json += "\"free\":";    json += String(ps.freeBytes);  json += ",";
json += "\"type\":\"";  json += ps.type;               json += "\",";
json += "\"mode\":\"";  json += ps.mode;               json += "\",";
json += "\"speed\":\""; json += ps.speed;              json += "\"";
json += "},";
```

**Explication** :
- `ps.enabled ? "true" : "false"` : **opérateur ternaire** (if compact)
  - Si `ps.enabled` est vrai → ajoute `"true"`
  - Sinon → ajoute `"false"`
- `String(ps.totalBytes)` : convertit le nombre en chaîne

**Résultat** :
```json
  "psram": {
    "enabled": true,
    "total": 8388608,
    "free": 7234567,
    "type": "OPI (8MB)",
    "mode": "8-line",
    "speed": "80MHz"
  },
```

---

```cpp
json += "\"board\":{";
json += "\"flash_bytes\":"; json += String(ESP.getFlashChipSize()); json += ",";
json += "\"heap_free\":";   json += String(ESP.getFreeHeap());      json += ",";
json += "\"chip_revision\":"; json += String(ESP.getChipRevision());
json += "}";
```

**Explication** :
- `ESP.getFlashChipSize()` : taille de la mémoire flash
- `ESP.getFreeHeap()` : mémoire heap libre (RAM interne)
- `ESP.getChipRevision()` : révision du chip ESP32

**Résultat** :
```json
  "board": {
    "flash_bytes": 16777216,
    "heap_free": 245632,
    "chip_revision": 1
  }
}
```

---

```cpp
json += "}";
return json;
```

**Explication** :
- Ferme l'objet JSON racine
- Retourne la chaîne JSON complète

---

## 4️⃣ Fonction : setup()

### 🎬 Initialisation du Serial

```cpp
Serial.begin(115200);
delay(200);
```

**Explication** :
- `Serial.begin(115200)` : Initialise la communication série à 115200 bauds
- `delay(200)` : Attend 200 ms pour stabiliser la connexion série

**Pourquoi 115200 ?** C'est un débit courant pour l'ESP32, assez rapide pour déboguer.

**Pourquoi `delay(200)` ?** Certains moniteurs série ont besoin d'un délai pour se synchroniser.

---

### 💡 Initialisation du NeoPixel

```cpp
pixel.begin();
```

**Explication** :
- Appelle la méthode `begin()` de l'objet `pixel`
- Initialise la LED NeoPixel et la met en mode "Idle" (blanc)

**Que se passe-t-il ?**
- Configure la broche GPIO 48
- Définit la luminosité
- Affiche la couleur par défaut

---

### 🖥️ Initialisation de l'écran OLED

```cpp
Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
```

**Explication** :
- Initialise le bus I2C avec les broches définies dans `board_config.h`
- `OLED_SDA_PIN = 15` : broche de données
- `OLED_SCL_PIN = 16` : broche d'horloge

**Pourquoi avant `oled.begin()` ?** Le bus I2C doit être initialisé avant de pouvoir communiquer avec l'écran.

---

```cpp
if (!oled.begin()) {
    pixel.set(StatusColor::ErrorApp);
    while (true) delay(1000);
}
```

**Explication** :
- Tente d'initialiser l'écran OLED
- `!oled.begin()` : si l'initialisation échoue (retourne `false`)
- Change la LED en **violet** (erreur application)
- **Boucle infinie** : le programme s'arrête là

**Pourquoi une boucle infinie ?** Sans écran, impossible de continuer. Mieux vaut arrêter proprement que de crasher.

---

### 🎨 Écran de démarrage (splash)

```cpp
oled.splash();
delay(2000);
```

**Explication** :
- Affiche l'écran de démarrage (nom du projet + version)
- Attend 2000 ms (2 secondes) pour que l'utilisateur le voie

---

### 📡 Connexion WiFi

```cpp
WifiManager wifi(pixel, oled);
bool ok = wifi.connect();

if (!ok) {
    return;
}
```

**Explication** :
1. Crée un objet `wifi` de type `WifiManager`
2. Passe les références `pixel` et `oled` au constructeur
3. Appelle `connect()` pour se connecter au WiFi
4. Si échec (`ok == false`), **sort de `setup()`**

**Que se passe-t-il en cas d'échec ?**
- `return;` sort de `setup()`
- Le programme passe à `loop()`
- Comme `loop()` est vide, rien ne se passe → LED rouge reste allumée

---

### 📊 Affichage de l'écran principal

```cpp
oled.mainScreen();
```

**Explication** :
- Affiche l'écran principal avec le SSID et l'IP
- Appelé uniquement si la connexion WiFi a réussi

---

### 📦 Initialisation du système de fichiers

```cpp
if (!LittleFS.begin(true)) {
    pixel.set(StatusColor::ErrorApp);
    return;
}
```

**Explication** :
- `LittleFS.begin(true)` : monte le système de fichiers
  - `true` : formater si nécessaire (première utilisation)
- Si échec, LED en violet (erreur app) et sortie

**Pourquoi ?** Sans système de fichiers, impossible de servir les pages web.

---

### 🌐 Configuration du serveur web

```cpp
server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
```

**Explication** :
- `serveStatic("/", LittleFS, "/")` : sert les fichiers du dossier racine `/` de LittleFS
- `.setDefaultFile("index.html")` : si on accède à `/`, servir `index.html`

**Analogie** : Comme configurer Apache ou Nginx pour servir un site web statique.

**Que se passe-t-il ?**
- Requête `http://192.168.1.42/` → répond avec `index.html`
- Requête `http://192.168.1.42/style.css` → répond avec `style.css`

---

```cpp
server.on("/api/info", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", buildInfoJson());
});
```

**Explication** :
- `server.on()` : définit un **endpoint** (point d'accès)
- `"/api/info"` : URL de l'endpoint
- `HTTP_GET` : méthode HTTP GET
- `[](...) { ... }` : **lambda** (fonction anonyme)

**C'est quoi une lambda ?**
- Une fonction sans nom, définie directement
- Ici, elle est appelée quand on accède à `/api/info`

**Que fait la lambda ?**
- `request->send(200, "application/json", buildInfoJson())`
  - Code 200 : HTTP OK
  - Type MIME : `application/json`
  - Contenu : le JSON construit par `buildInfoJson()`

**Utilisation** :
- Requête `http://192.168.1.42/api/info` → répond avec les infos en JSON

---

```cpp
server.begin();
```

**Explication** :
- Démarre le serveur web
- À partir de ce moment, le serveur écoute sur le port 80

---

## 5️⃣ Fonction : loop()

```cpp
void loop() {
    // AsyncWebServer → loop vide
}
```

**Explication** :
- La fonction `loop()` est **vide** !
- C'est normal : le serveur web asynchrone gère tout en arrière-plan

**Comment ça marche ?**
- `ESPAsyncWebServer` utilise des interruptions et des tâches FreeRTOS
- Les requêtes HTTP sont traitées automatiquement
- Pas besoin de code dans `loop()`

**Analogie** : Comme un serveur web Apache qui tourne en background, pas besoin de le surveiller.

---

## 📊 Flux d'exécution complet

```
┌─────────────────────────────┐
│  Démarrage ESP32            │
└──────────┬──────────────────┘
           ▼
┌─────────────────────────────┐
│  setup()                    │
│  ├─ Serial.begin()          │ ← Initialiser communication série
│  ├─ pixel.begin()           │ ← Initialiser LED
│  ├─ Wire.begin()            │ ← Initialiser bus I2C
│  ├─ oled.begin()            │ ← Initialiser écran OLED
│  ├─ oled.splash()           │ ← Afficher écran de démarrage
│  │   └─ delay(2000)         │ ← Attendre 2 secondes
│  ├─ wifi.connect()          │ ← Connecter au WiFi
│  │   ├─ Scan réseau         │
│  │   ├─ Connexion           │
│  │   └─ Enregistrer IP      │
│  ├─ oled.mainScreen()       │ ← Afficher écran principal
│  ├─ LittleFS.begin()        │ ← Monter système de fichiers
│  ├─ server.serveStatic()    │ ← Configurer serveur (fichiers)
│  ├─ server.on("/api/info")  │ ← Configurer API JSON
│  └─ server.begin()          │ ← Démarrer serveur web
└──────────┬──────────────────┘
           ▼
┌─────────────────────────────┐
│  loop()                     │ ← Boucle vide (serveur asynchrone)
│  (vide)                     │
└──────────┬──────────────────┘
           │
           └─────────► Répétition infinie
```

---

## 🎯 Points clés à retenir

### Variables globales
- `server` : serveur web qui vit pendant tout le programme
- `pixel`, `oled` : objets matériels utilisés dans plusieurs fonctions

### Fonction `buildInfoJson()`
- Construit un JSON avec les infos système
- Utilisé par l'endpoint `/api/info`
- Optimisé avec `reserve()` pour éviter les réallocations

### Fonction `setup()`
1. **Initialisation** : Serial, LED, écran
2. **Connexion WiFi** : avec feedback visuel (LED + écran)
3. **Montage filesystem** : pour servir les fichiers web
4. **Configuration serveur** : fichiers statiques + API JSON
5. **Démarrage serveur** : écoute sur port 80

### Fonction `loop()`
- Vide car serveur asynchrone
- Le serveur gère les requêtes automatiquement en arrière-plan

---

**Prochaine étape** : Lisez `05_configuration.md` pour comprendre les fichiers de configuration (GPIO, timeouts, secrets).
