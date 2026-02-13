# Concepts C++ utilisés dans ce projet

## 📚 Introduction

Ce document explique les concepts C++ que vous rencontrez dans ce projet. Chaque concept est expliqué simplement, avec des analogies et des exemples.

---

## 1️⃣ Classes et Objets

### 🤔 Qu'est-ce qu'une classe ?

Une **classe** est un **modèle** (blueprint) qui décrit :
- Des **données** (variables membres)
- Des **comportements** (fonctions membres = méthodes)

**Analogie** : Une classe est comme un plan de construction d'une maison.

### 🏠 Exemple : La classe NeoPixelStatus

```cpp
class NeoPixelStatus {
public:
    void begin();
    void set(StatusColor status);

private:
    Adafruit_NeoPixel pixel;
};
```

**Décryptage** :
- `class NeoPixelStatus` : Définit une nouvelle classe nommée `NeoPixelStatus`
- `public:` : Méthodes accessibles de l'extérieur
- `private:` : Données accessibles uniquement à l'intérieur de la classe

### 🔨 Créer un objet (instance)

```cpp
NeoPixelStatus pixel;  // Créer un objet de type NeoPixelStatus
```

**Analogie** : La classe est le plan, l'objet `pixel` est la maison construite à partir du plan.

### 📞 Utiliser un objet

```cpp
pixel.begin();                      // Appeler la méthode begin()
pixel.set(StatusColor::Connected);  // Appeler la méthode set()
```

**Le point `.`** signifie "accéder à un membre de l'objet".

---

## 2️⃣ Références et pointeurs

### 🔗 Les références (`&`)

Une **référence** est un **alias** (surnom) pour un objet existant.

```cpp
void WifiManager(NeoPixelStatus& px) {
    // px est une référence, pas une copie
    px.set(StatusColor::Scanning);  // Modifie l'objet original
}
```

**Pourquoi utiliser `&` ?**
- ✅ Évite de copier l'objet (économie de mémoire)
- ✅ Modifie l'objet original (pas une copie)

**Analogie** : Au lieu de photocopier un document, on pointe vers le document original.

### 🆚 Avec ou sans référence

```cpp
// SANS référence (copie)
void fonction1(NeoPixelStatus px) {
    px.set(...);  // Modifie la COPIE, pas l'original
}

// AVEC référence (alias)
void fonction2(NeoPixelStatus& px) {
    px.set(...);  // Modifie l'ORIGINAL
}
```

**Dans ce projet** : On utilise toujours des références pour éviter les copies coûteuses.

### 🧭 Les pointeurs (`*`)

Un **pointeur** est une variable qui contient l'**adresse mémoire** d'un objet.

```cpp
NeoPixelStatus* ptr = &pixel;  // ptr contient l'adresse de pixel
ptr->set(StatusColor::Idle);   // Utiliser -> pour accéder
```

**Différence avec les références** :
- Les pointeurs peuvent être `nullptr` (vide)
- Les références sont toujours valides
- Les références sont plus sûres et simples

**Dans ce projet** : On préfère les références aux pointeurs quand possible (règle de sécurité).

---

## 3️⃣ Namespaces

### 🗂️ Qu'est-ce qu'un namespace ?

Un **namespace** est un **dossier logique** pour organiser le code et éviter les conflits de noms.

**Problème sans namespace** :
```cpp
// Fichier A
int TIMEOUT = 1000;

// Fichier B
int TIMEOUT = 5000;  // ERREUR : conflit !
```

**Solution avec namespace** :
```cpp
// Fichier A
namespace ModuleA {
    int TIMEOUT = 1000;
}

// Fichier B
namespace ModuleB {
    int TIMEOUT = 5000;
}

// Utilisation
int x = ModuleA::TIMEOUT;  // 1000
int y = ModuleB::TIMEOUT;  // 5000
```

### 📦 Exemple dans ce projet

```cpp
namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;
}

// Utilisation
int timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
```

**`::` (opérateur de résolution de portée)** : "Accéder à quelque chose dans le namespace `Config`"

### 💡 Pourquoi des namespaces ?

- ✅ Évite les conflits de noms
- ✅ Organise le code logiquement
- ✅ Rend le code plus lisible

---

## 4️⃣ Constantes et constexpr

### 📌 `const` : Valeur non modifiable

```cpp
const int MAX_RETRIES = 10;
MAX_RETRIES = 20;  // ERREUR : ne peut pas modifier une const
```

**Utilité** : Protège contre les modifications accidentelles.

### ⚡ `constexpr` : Constante évaluée à la compilation

```cpp
constexpr uint32_t TIMEOUT = 20000;
```

**Différence avec `const`** :
- `const` : valeur non modifiable (peut être calculée à l'exécution)
- `constexpr` : valeur **calculée à la compilation** (plus rapide, moins de mémoire)

**Dans ce projet** : On utilise `constexpr` pour toutes les configurations fixes.

### 🔢 Types numériques

```cpp
uint32_t nombre = 20000;
```

**Décomposition** :
- `u` : **unsigned** (non signé, seulement positif)
- `int` : entier
- `32` : **32 bits** (peut aller de 0 à 4 294 967 295)
- `_t` : convention pour les types standards

**Pourquoi ?** C'est plus précis et portable que `int` (dont la taille varie selon la plateforme).

---

## 5️⃣ Enum et enum class

### 🎨 `enum` : Énumération

Une **énumération** définit un ensemble de **valeurs nommées**.

```cpp
enum Couleur {
    Rouge,
    Vert,
    Bleu
};

Couleur c = Rouge;
```

**Problème** : Les noms polluent l'espace global.

### 🎯 `enum class` : Énumération sûre

```cpp
enum class StatusColor {
    Idle,
    Scanning,
    Connecting,
    Connected
};

StatusColor etat = StatusColor::Idle;  // Doit préfixer avec StatusColor::
```

**Avantages** :
- ✅ Pas de pollution de l'espace de noms
- ✅ Plus sûr (pas de conversion implicite en int)
- ✅ Plus lisible

**Dans ce projet** : On utilise toujours `enum class`.

---

## 6️⃣ Structures (struct)

### 📦 Qu'est-ce qu'une structure ?

Une **structure** regroupe plusieurs variables liées en une seule entité.

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

**Utilisation** :
```cpp
PsramInfo info;
info.enabled = true;
info.totalBytes = 8388608;

// Ou initialisation directe
PsramInfo info2 = {
    .enabled = true,
    .totalBytes = 8388608,
    .freeBytes = 7000000,
    .type = "OPI",
    .mode = "8-line",
    .speed = "80MHz"
};
```

### 🆚 struct vs class

| Aspect | `struct` | `class` |
|--------|----------|---------|
| **Visibilité par défaut** | `public` | `private` |
| **Utilisation typique** | Regrouper des données | Encapsuler logique + données |

**Dans ce projet** : On utilise `struct` pour les données simples (comme `PsramInfo`).

---

## 7️⃣ std::string vs String

### 🔤 Deux types de chaînes de caractères

#### Arduino `String` (déconseillé)

```cpp
String s = "Hello";
s += " World";
```

**Problèmes** :
- ❌ Fragmentation de la mémoire heap
- ❌ Lent et imprévisible
- ❌ Peut causer des crashs sur ESP32

#### C++ `std::string` (recommandé)

```cpp
std::string s = "Hello";
s += " World";
```

**Avantages** :
- ✅ Standard C++ (portable)
- ✅ Gestion mémoire plus efficace
- ✅ Plus sûr

**Dans ce projet** : On utilise **toujours** `std::string`.

### 🔄 Conversions

```cpp
std::string s = "Hello";
const char* cstr = s.c_str();  // Pour les API C (comme Serial.print)

String arduinoStr = String(s.c_str());  // Si vraiment nécessaire
```

---

## 8️⃣ std::string_view

### 👀 Vue non-propriétaire

`std::string_view` est une **vue** sur une chaîne de caractères, sans copie.

```cpp
constexpr std::string_view PROJECT_NAME = "Mon Projet";
```

**Avantages** :
- ✅ Pas de copie (économie de mémoire)
- ✅ Peut être `constexpr` (évalué à la compilation)
- ✅ Très rapide

**Dans ce projet** : Utilisé pour `PROJECT_NAME` et `PROJECT_VERSION` car ce sont des constantes compilées.

### 🆚 std::string vs std::string_view

| Type | Propriétaire | Copie | Modifiable |
|------|--------------|-------|------------|
| `std::string` | Oui | Oui | Oui |
| `std::string_view` | Non (vue) | Non | Non |

---

## 9️⃣ Singleton Pattern

### 🏛️ Une seule instance globale

Le **Singleton** garantit qu'il n'existe qu'**une seule instance** d'une classe.

**Exemple : ConfigState**

```cpp
class ConfigState {
public:
    static ConfigState& instance() {
        static ConfigState inst;  // Créée UNE SEULE FOIS
        return inst;
    }

    void setIp(const std::string& ip) { ipAddress = ip; }
    const std::string& ip() const { return ipAddress; }

private:
    ConfigState() = default;  // Constructeur privé

    std::string ipAddress;
    std::string connectedSsid;
};
```

**Utilisation** :
```cpp
ConfigState::instance().setIp("192.168.1.100");
std::string ip = ConfigState::instance().ip();
```

**Explications** :

1. **`static ConfigState& instance()`** : Fonction statique (pas besoin d'objet pour l'appeler)
2. **`static ConfigState inst;`** : Variable locale statique (créée une seule fois, vit jusqu'à la fin du programme)
3. **`ConfigState() = default;`** : Constructeur par défaut, mais **privé** → impossible de créer un objet directement

**Pourquoi ?**
- ✅ État global accessible partout
- ✅ Une seule instance garantie
- ✅ Création paresseuse (lazy initialization)

**Quand l'utiliser ?**
- Configuration globale (comme ici)
- Gestionnaires de ressources partagées

---

## 🔟 Includes et Pragma Once

### 📥 `#include` : Inclusion de fichiers

```cpp
#include <Arduino.h>       // Bibliothèque système (chevrons <>)
#include "wifi_manager.h"  // Fichier local (guillemets "")
```

**Comment ça marche ?**
Le préprocesseur **copie-colle** le contenu du fichier inclus à cet endroit.

### 🛡️ `#pragma once` : Protection contre les doubles inclusions

**Problème** :
```cpp
// fichier A.h
struct Data { int x; };

// fichier B.h
#include "A.h"

// fichier C.cpp
#include "A.h"
#include "B.h"  // A.h inclus 2 fois → ERREUR : redéfinition de Data
```

**Solution** :
```cpp
// A.h
#pragma once

struct Data { int x; };
```

**Effet** : Le fichier A.h ne sera inclus qu'**une seule fois** même s'il est demandé plusieurs fois.

**Dans ce projet** : Tous les fichiers `.h` commencent par `#pragma once`.

---

## 1️⃣1️⃣ Constructeurs et listes d'initialisation

### 🏗️ Constructeur

Un **constructeur** est une fonction spéciale appelée lors de la **création** d'un objet.

```cpp
class WifiManager {
public:
    WifiManager(NeoPixelStatus& px, OledDisplay& ol);
};
```

### 📋 Liste d'initialisation

```cpp
WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& ol)
    : pixel(px), display(ol) {
    // Corps du constructeur (peut être vide)
}
```

**Explication** :
- `: pixel(px), display(ol)` : **liste d'initialisation**
- **Avant** l'accolade `{`, on initialise les membres
- Plus efficace que l'affectation dans le corps

**Équivalent moins efficace** :
```cpp
WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& ol) {
    pixel = px;   // Affectation (moins efficace)
    display = ol;
}
```

**Pourquoi la liste d'initialisation ?**
- ✅ Obligatoire pour les références (on ne peut pas affecter une référence)
- ✅ Plus efficace (initialisation directe)
- ✅ Ordre d'initialisation clair

---

## 1️⃣2️⃣ Public, Private, Protected

### 🔓 Niveaux d'accès

```cpp
class Exemple {
public:
    void methodePublique();    // Accessible partout

private:
    void methodePrivee();      // Accessible uniquement dans la classe
    int donneePrivee;

protected:
    void methodeProtegee();    // Accessible dans la classe et les classes dérivées
};
```

**Règle d'or** :
- Données → **private**
- Méthodes d'interface → **public**
- Méthodes d'implémentation → **private**

**Pourquoi ?** **Encapsulation** : cacher les détails internes, exposer uniquement l'interface.

---

## 1️⃣3️⃣ Macros (#define)

### 🔧 Définir une constante

```cpp
#define OLED_SDA_PIN 15
#define OLED_SCL_PIN 16
```

**Comment ça marche ?**
Le préprocesseur **remplace** toutes les occurrences de `OLED_SDA_PIN` par `15` avant la compilation.

**Utilisation** :
```cpp
Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
// Devient après préprocessing :
Wire.begin(15, 16);
```

### 🆚 #define vs constexpr

| Aspect | `#define` | `constexpr` |
|--------|-----------|-------------|
| **Type** | Aucun (texte brut) | Type vérifié |
| **Portée** | Globale | Respecte les scopes |
| **Débogage** | Difficile | Facile |

**Quand utiliser `#define` ?**
- Configuration matérielle (GPIO, ports)
- Traditionnellement utilisé en C embarqué

**Dans ce projet** :
- `#define` pour les GPIO (board_config.h)
- `constexpr` pour les autres constantes (config.h)

---

## 1️⃣4️⃣ Type Casting

### 🔀 Conversion de types

```cpp
float progress = (float)(millis() - start) / timeout;
```

**Explication** :
- `(float)` : **cast** (conversion forcée) vers `float`
- Nécessaire car `millis()` retourne un `unsigned long` (entier)
- Sans cast, division entière → perte de précision

**Exemple** :
```cpp
int a = 5;
int b = 2;
int resultat = a / b;        // 2 (division entière)
float resultat2 = (float)a / b;  // 2.5 (division réelle)
```

---

## 1️⃣5️⃣ Boolean et return

### ✅ Type `bool`

```cpp
bool connected = true;
bool error = false;
```

**Valeurs** : `true` (vrai) ou `false` (faux)

### 🔙 `return`

```cpp
bool WifiManager::connect() {
    if (erreur) {
        return false;  // Sort de la fonction et retourne false
    }
    
    // ...
    
    return true;  // Sort de la fonction et retourne true
}
```

**Utilisation** :
```cpp
bool ok = wifi.connect();

if (ok) {
    Serial.println("Connecté !");
} else {
    Serial.println("Erreur de connexion");
}
```

---

## 📊 Résumé des concepts

| Concept | Utilisation dans le projet |
|---------|---------------------------|
| **Classes** | NeoPixelStatus, OledDisplay, WifiManager |
| **Références (`&`)** | Passer des objets sans copie |
| **Namespaces** | Config, ProjectInfo, ConfigConstants |
| **constexpr** | Constantes de configuration |
| **enum class** | StatusColor (états de la LED) |
| **struct** | PsramInfo (regrouper données) |
| **std::string** | Chaînes de caractères (IP, SSID) |
| **Singleton** | ConfigState (état global unique) |
| **#pragma once** | Protection des headers |
| **#define** | Macros pour GPIO |

---

**Prochaine étape** : Lisez `04_main_explique.md` pour une analyse détaillée ligne par ligne du fichier main.cpp.
