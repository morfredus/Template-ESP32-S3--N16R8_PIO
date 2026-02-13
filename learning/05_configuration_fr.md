# Fichiers de configuration

## 📋 Vue d'ensemble

La configuration est **séparée** du code principal pour faciliter :
- ✅ Les modifications sans toucher au code
- ✅ L'adaptation à différentes cartes
- ✅ La sécurité (secrets non partagés)

**Fichiers de configuration** :
1. `board_config.h` - Configuration matérielle (GPIO)
2. `config.h` - Configuration logicielle (timeouts, paramètres)
3. `secrets.h` - Identifiants sensibles (WiFi)
4. `config_constants.h` - Constantes dérivées (dans src/utils)
5. `config_state.h` - État de la configuration à l'exécution

---

## 1️⃣ board_config.h (Configuration matérielle)

### 📍 Localisation
```
include/board_config.h
```

### 🎯 Rôle

Définit **toutes les broches GPIO** et paramètres matériels spécifiques à la carte.

### 📄 Contenu complet

```cpp
#pragma once

// OLED I2C (recommandé par Espressif)
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
#define OLED_WIDTH    128
#define OLED_HEIGHT   64

// NeoPixel intégré
#define NEOPIXEL_PIN        48
#define NEOPIXEL_BRIGHTNESS 38  // ~15%

// Web server
#define WEB_SERVER_PORT 80
```

---

### 📖 Explication détaillée

#### OLED I2C

```cpp
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
```

**Rôle** : Définit les broches GPIO pour le bus I2C de l'écran OLED.

**C'est quoi I2C ?**
- **I**nter-**I**ntegrated **C**ircuit
- Bus de communication à 2 fils :
  - **SDA** (Serial Data) : ligne de données
  - **SCL** (Serial Clock) : ligne d'horloge

**Pourquoi ces broches ?**
- GPIO 15 et 16 sont recommandés par Espressif pour l'I2C sur l'ESP32-S3
- Évite les conflits avec d'autres périphériques internes

**Analogie** : Comme un câble USB avec 2 fils (données + horloge) pour connecter plusieurs appareils.

---

```cpp
#define OLED_WIDTH    128
#define OLED_HEIGHT   64
```

**Rôle** : Dimensions de l'écran OLED en pixels.

**Pourquoi définir en constante ?**
- ✅ Facile de changer si vous utilisez un autre écran (ex: 128x32)
- ✅ Utilisé pour calculer les positions d'affichage

**Utilisation dans le code** :
```cpp
display.begin(OLED_WIDTH, OLED_HEIGHT);
int centreX = OLED_WIDTH / 2;  // Calculer le centre
```

---

#### NeoPixel

```cpp
#define NEOPIXEL_PIN        48
```

**Rôle** : Broche GPIO connectée à la LED NeoPixel intégrée.

**Pourquoi GPIO 48 ?**
- Sur l'ESP32-S3-DevKitC-1, la LED RGB intégrée est câblée sur GPIO 48
- C'est fixe sur cette carte, impossible de changer

---

```cpp
#define NEOPIXEL_BRIGHTNESS 38  // ~15%
```

**Rôle** : Luminosité de la LED (0-255).

**Pourquoi 38 (~15%) ?**
- Les LEDs NeoPixel sont **très lumineuses** à 100%
- 38/255 ≈ 15% : confortable pour les yeux
- Économise aussi un peu d'énergie

**Comment ça fonctionne ?**
```cpp
pixel.setBrightness(NEOPIXEL_BRIGHTNESS);  // Applique la luminosité
```

---

#### Web Server

```cpp
#define WEB_SERVER_PORT 80
```

**Rôle** : Port réseau pour le serveur web.

**Pourquoi 80 ?**
- Port standard HTTP (comme les sites web normaux)
- Permet d'accéder sans spécifier le port : `http://192.168.1.42/` au lieu de `http://192.168.1.42:8080/`

**Autres ports courants** :
- 80 : HTTP (non sécurisé)
- 443 : HTTPS (sécurisé)
- 8080 : HTTP alternatif (souvent pour développement)

---

### 🛡️ RÈGLE IMPORTANTE

**❌ NE PAS MODIFIER ce fichier sans raison valide !**

**Pourquoi ?**
- Les GPIO sont liés au **matériel physique**
- Modifier sans connaissance → risque de conflit ou dysfonctionnement
- Les autres développeurs comptent sur ces valeurs

**Quand modifier ?**
- Changement de carte ESP32
- Ajout d'un nouveau périphérique (avec confirmation de GPIO libres)
- Résolution d'un conflit matériel

---

## 2️⃣ config.h (Configuration logicielle)

### 📍 Localisation
```
include/config.h
```

### 🎯 Rôle

Définit les **paramètres de comportement** du programme (timeouts, délais, etc.).

### 📄 Contenu complet

```cpp
#pragma once
#include <stdint.h>

namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000; // 20 s total
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;   // 0,5 s par tentative
    constexpr uint32_t SPLASH_SCREEN_MS        = 2000;  // splash OLED
}
```

---

### 📖 Explication détaillée

#### Namespace Config

```cpp
namespace Config {
    // ...
}
```

**Pourquoi un namespace ?**
- ✅ Organisation : toutes les configs au même endroit
- ✅ Évite les conflits : `Config::WIFI_TIMEOUT` vs `WIFI_TIMEOUT` global
- ✅ Lisibilité : le préfixe `Config::` indique que c'est un paramètre

---

#### Timeout de connexion WiFi

```cpp
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000; // 20 s total
```

**Rôle** : Durée maximale (en millisecondes) pour tenter de se connecter au WiFi.

**Décomposition** :
- `constexpr` : constante évaluée à la compilation
- `uint32_t` : entier non signé 32 bits (0 à 4 294 967 295)
- `= 20000` : 20 000 millisecondes = 20 secondes

**Pourquoi 20 secondes ?**
- Assez long pour donner le temps au WiFi de se connecter
- Pas trop long pour ne pas bloquer indéfiniment

**Utilisation** :
```cpp
unsigned long timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
if (millis() - start > timeout) {
    // Timeout dépassé, abandonner
}
```

---

#### Délai entre les tentatives

```cpp
constexpr uint32_t WIFI_RETRY_DELAY_MS = 500;   // 0,5 s par tentative
```

**Rôle** : Délai (en millisecondes) entre chaque vérification de l'état WiFi.

**Pourquoi 500 ms ?**
- Vérifie l'état toutes les 0,5 secondes
- Permet de mettre à jour l'écran et la LED régulièrement
- Pas trop rapide (économie CPU), pas trop lent (réactivité)

**Utilisation** :
```cpp
while (WiFi.status() != WL_CONNECTED) {
    // ...
    delay(Config::WIFI_RETRY_DELAY_MS);  // Attendre avant de réessayer
}
```

---

#### Durée du splash screen

```cpp
constexpr uint32_t SPLASH_SCREEN_MS = 2000;  // splash OLED
```

**Rôle** : Durée d'affichage (en millisecondes) de l'écran de démarrage.

**Pourquoi 2 secondes ?**
- Assez long pour lire le nom et la version
- Pas trop long pour ne pas ralentir le démarrage

**Utilisation** :
```cpp
oled.splash();
delay(Config::SPLASH_SCREEN_MS);  // Afficher pendant 2 secondes
```

---

### 💡 Avantages de cette approche

**Centralisation** :
- Tous les paramètres au même endroit
- Facile de les ajuster

**Lisibilité** :
```cpp
// ❌ Difficile à comprendre
delay(2000);

// ✅ Clair et explicite
delay(Config::SPLASH_SCREEN_MS);
```

**Facilité de maintenance** :
- Pour changer le timeout WiFi, modifier **un seul endroit**
- Pas besoin de chercher dans tout le code

---

## 3️⃣ secrets.h (Identifiants sensibles)

### 📍 Localisation
```
include/secrets.h
```

### 🎯 Rôle

Contient les **identifiants sensibles** (SSID WiFi, mot de passe).

### 📄 Structure typique

```cpp
#pragma once

// WiFi credentials
#define WIFI_SSID     "Votre_SSID_WiFi"
#define WIFI_PASSWORD "Votre_MotDePasse"
```

**⚠️ ATTENTION** :
- ❌ NE JAMAIS partager ce fichier
- ❌ NE JAMAIS le pousser sur GitHub/GitLab
- 🛡️ Il est dans `.gitignore` pour éviter les commits accidentels

---

### 🔒 Sécurité

**Pourquoi un fichier séparé ?**
- ✅ Le code peut être partagé sans exposer les identifiants
- ✅ Chaque développeur a ses propres identifiants
- ✅ Facilite le versionnage (secrets pas dans Git)

**Fichier example fourni** :
```
include/secrets_example.h
```

**Contenu** :
```cpp
#pragma once

// WiFi credentials
#define WIFI_SSID     "YOUR_SSID_HERE"
#define WIFI_PASSWORD "YOUR_PASSWORD_HERE"
```

**Usage** :
1. Copier `secrets_example.h` → `secrets.h`
2. Modifier avec vos vrais identifiants
3. `secrets.h` est ignoré par Git

---

### 📖 Utilisation dans le code

```cpp
// Dans wifi_manager.cpp
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
```

**Comment ça marche ?**
- Le préprocesseur remplace `WIFI_SSID` par la valeur définie
- Équivaut à :
```cpp
WiFi.begin("Votre_SSID_WiFi", "Votre_MotDePasse");
```

---

## 4️⃣ config_constants.h (Constantes dérivées)

### 📍 Localisation
```
src/utils/config_constants.h
```

### 🎯 Rôle

Rend accessibles les constantes **injectées à la compilation** (depuis `platformio.ini`).

### 📄 Contenu complet

```cpp
#pragma once
#include <string_view>

namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME    = PROJECT_NAME;
    constexpr std::string_view PROJECT_VERSION = PROJECT_VERSION;
}
```

---

### 📖 Explication détaillée

#### Origine : platformio.ini

```ini
[env:esp32-s3-devkitc-1]
build_flags = 
    -D PROJECT_NAME='"Template-ESP32-S3"'
    -D PROJECT_VERSION='"1.0.0"'
```

**Explication** :
- `-D PROJECT_NAME='"..."'` : définit une macro `PROJECT_NAME` avec la valeur entre guillemets
- Ces macros sont disponibles **partout** dans le code C++

---

#### Wrapping dans un namespace

```cpp
namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME = PROJECT_NAME;
}
```

**Pourquoi ce wrapper ?**
1. **Organisation** : Regrouper dans un namespace clair
2. **Type safe** : `std::string_view` au lieu d'une macro brute
3. **Lisibilité** : `ConfigConstants::PROJECT_NAME` vs `PROJECT_NAME` (macro)

**C'est quoi `std::string_view` ?**
- Vue **non-propriétaire** sur une chaîne
- Pas de copie, juste un pointeur + taille
- Très efficace pour les constantes

---

### 💡 Utilisation

```cpp
#include "utils/config_constants.h"

Serial.print("Projet : ");
Serial.println(ConfigConstants::PROJECT_NAME.data());

Serial.print("Version : ");
Serial.println(ConfigConstants::PROJECT_VERSION.data());
```

**`.data()`** : retourne un `const char*` (format C) pour les API Arduino.

---

## 5️⃣ config_state.h (État à l'exécution)

### 📍 Localisation
```
src/utils/config_state.h
```

### 🎯 Rôle

Stocke l'**état de la configuration à l'exécution** (valeurs qui changent pendant le programme).

### 📄 Contenu complet

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

### 📖 Explication détaillée

#### Pattern Singleton

```cpp
static ConfigState& instance() {
    static ConfigState inst;
    return inst;
}
```

**Rôle** : Garantit qu'il n'existe qu'**une seule instance** de `ConfigState`.

**Comment ça marche ?**
1. `static ConfigState inst;` : variable locale statique, créée **une seule fois**
2. `return inst;` : retourne toujours la même instance

**Utilisation** :
```cpp
ConfigState::instance().setIp("192.168.1.42");
std::string ip = ConfigState::instance().ip();
```

---

#### Getters et Setters

```cpp
void setIp(const std::string& ip) { ipAddress = ip; }
const std::string& ip() const { return ipAddress; }
```

**Rôle** :
- `setIp()` : **setter** (définit l'IP)
- `ip()` : **getter** (récupère l'IP)

**Pourquoi des accesseurs ?**
- ✅ Encapsulation : contrôle l'accès aux données
- ✅ Flexibilité : possibilité d'ajouter de la logique (validation, logging, etc.)

**`const` après la fonction** :
```cpp
const std::string& ip() const { ... }
```
- Indique que la fonction **ne modifie pas** l'objet
- Permet d'appeler la fonction sur des objets constants

---

#### Données privées

```cpp
private:
    ConfigState() = default;

    std::string ipAddress;
    std::string connectedSsid;
```

**Rôle** :
- `ConfigState() = default;` : constructeur privé (empêche la création directe d'instances)
- `ipAddress`, `connectedSsid` : données stockées

**Pourquoi privé ?**
- Force l'utilisation du singleton (`instance()`)
- Impossible de créer un autre `ConfigState`

---

### 💡 Utilisation complète

#### 1. Enregistrer l'état (après connexion WiFi)

```cpp
// Dans wifi_manager.cpp
ConfigState::instance().setIp(WiFi.localIP().toString().c_str());
ConfigState::instance().setSsid(WIFI_SSID);
```

#### 2. Lire l'état (dans main.cpp)

```cpp
// Dans buildInfoJson()
json += "\"ssid\":\"";
json += ConfigState::instance().ssid().c_str();
json += "\",";

json += "\"ip\":\"";
json += ConfigState::instance().ip().c_str();
json += "\"";
```

#### 3. Afficher l'état (sur l'écran OLED)

```cpp
// Dans oled_display.cpp
display.print(ConfigState::instance().ssid().c_str());
display.print(ConfigState::instance().ip().c_str());
```

---

### 🆚 config.h vs config_state.h

| Fichier | Type | Valeurs | Moment |
|---------|------|---------|--------|
| **config.h** | Constantes | Fixes (timeouts, délais) | Compilation |
| **config_state.h** | État | Variables (IP, SSID) | Exécution |

**Analogie** :
- `config.h` : Les règles du jeu (fixes)
- `config_state.h` : Le score actuel (change pendant le jeu)

---

## 📊 Récapitulatif des fichiers de configuration

| Fichier | Localisation | Rôle | Modifiable |
|---------|--------------|------|------------|
| **board_config.h** | `include/` | GPIO, ports | ❌ Rarement |
| **config.h** | `include/` | Timeouts, paramètres | ✅ Souvent |
| **secrets.h** | `include/` | WiFi, mots de passe | ✅ Par utilisateur |
| **config_constants.h** | `src/utils/` | Nom, version (depuis ini) | ❌ Jamais |
| **config_state.h** | `src/utils/` | IP, SSID (runtime) | ❌ Jamais (code) |

---

## 🎯 Bonnes pratiques

### ✅ À FAIRE

- Centraliser les paramètres dans `config.h`
- Utiliser des noms explicites (`WIFI_TIMEOUT_MS` plutôt que `TIMEOUT`)
- Commenter les valeurs non évidentes
- Utiliser `constexpr` pour les valeurs fixes

### ❌ À ÉVITER

- Hardcoder des valeurs directement dans le code
- Modifier `board_config.h` sans raison valable
- Pousser `secrets.h` sur un dépôt public
- Utiliser des valeurs magiques sans nom (`delay(2000);` → pourquoi 2000 ?)

---

## 🔗 Comment tout est relié

```
platformio.ini
   │
   ├─ PROJECT_NAME ──────────────┐
   └─ PROJECT_VERSION ───────────┤
                                 ▼
                         config_constants.h (wrapping)
                                 │
                                 ▼
                         project_info.h (alias)
                                 │
                                 ▼
                         main.cpp (usage)


board_config.h ───► modules (GPIO)
config.h ──────────► managers (timeouts)
secrets.h ─────────► wifi_manager (credentials)
config_state.h ────► main.cpp, oled, api (état runtime)
```

---

## 🎓 Conclusion

Les fichiers de configuration permettent de :
- ✅ **Séparer** la configuration du code
- ✅ **Adapter** facilement le projet à différentes cartes
- ✅ **Sécuriser** les identifiants sensibles
- ✅ **Maintenir** le code facilement

**Règle d'or** : Quand vous hésitez entre hardcoder une valeur ou la mettre en configuration, **mettez-la en configuration** !

---

**Fin de la documentation !** Vous avez maintenant une compréhension complète du projet. 🎉
