# Structure du projet ESP32

## 📂 Vue d'ensemble

```
Template-ESP32-S3--N16R8_PIO/
│
├── 📁 src/                    ← CODE SOURCE PRINCIPAL
│   ├── main.cpp               ← Point d'entrée du programme
│   ├── managers/              ← Gestionnaires de haut niveau
│   ├── modules/               ← Modules matériels (écran, LED, etc.)
│   └── utils/                 ← Utilitaires et helpers
│
├── 📁 include/                ← FICHIERS DE CONFIGURATION
│   ├── board_config.h         ← Configuration des broches GPIO
│   ├── config.h               ← Configuration générale
│   └── secrets.h              ← Identifiants WiFi (NE PAS PARTAGER)
│
├── 📁 data/                   ← FICHIERS WEB (HTML, CSS, JS)
│   ├── index.html             ← Page web servie par l'ESP32
│   └── style.css              ← Styles de la page web
│
├── 📁 docs/                   ← DOCUMENTATION UTILISATEUR
│   └── (guides, manuels)
│
├── 📁 lib/                    ← BIBLIOTHÈQUES LOCALES (si besoin)
│
├── 📁 scripts/                ← SCRIPTS PYTHON UTILITAIRES
│   └── auto_fs.py             ← Upload automatique du système de fichiers
│
├── 📁 test/                   ← TESTS UNITAIRES
│
├── platformio.ini             ← Configuration PlatformIO
└── README.md                  ← Documentation du projet
```

---

## 🎯 Le dossier `src/` (code source)

C'est **le cœur du programme**. Tout le code C++ est ici.

### 📄 main.cpp

**Rôle** : Point d'entrée du programme, orchestre tous les modules.

**Contient** :
- `setup()` : Fonction d'initialisation (exécutée une fois)
- `loop()` : Fonction de boucle principale (dans ce projet, elle est vide)

**Analogie** : C'est le chef d'orchestre qui fait jouer tous les musiciens au bon moment.

---

### 📁 managers/ (gestionnaires)

Les **managers** sont responsables de **coordonner plusieurs modules** pour accomplir une tâche complexe.

#### wifi_manager/

```
wifi_manager/
├── wifi_manager.h      ← Déclaration de la classe WifiManager
└── wifi_manager.cpp    ← Implémentation
```

**Rôle** : Gérer la connexion WiFi.

**Pourquoi un manager ?** Connecter le WiFi implique :
- Interagir avec le pixel (afficher l'état)
- Interagir avec l'écran OLED (afficher la progression)
- Gérer les tentatives de connexion
- Enregistrer l'état dans ConfigState

C'est un **orchestrateur** qui utilise plusieurs modules.

---

### 📁 modules/ (modules matériels)

Les **modules** sont des composants **autonomes** qui contrôlent un élément matériel spécifique.

#### neopixel_status/

```
neopixel_status/
├── neopixel_status.h      ← Déclaration
└── neopixel_status.cpp    ← Implémentation
```

**Rôle** : Contrôler la LED NeoPixel intégrée pour afficher l'état du système.

**Fonctionnalités** :
- `begin()` : Initialiser la LED
- `set(StatusColor)` : Changer la couleur selon l'état

**Pourquoi un module ?** La LED est un composant matériel qui peut être réutilisé dans d'autres projets.

#### oled_display/

```
oled_display/
├── oled_display.h      ← Déclaration
└── oled_display.cpp    ← Implémentation
```

**Rôle** : Contrôler l'écran OLED SSD1306 128x64.

**Fonctionnalités** :
- `begin()` : Initialiser l'écran
- `splash()` : Afficher l'écran de démarrage
- `wifiProgress(float)` : Afficher la progression de connexion WiFi
- `mainScreen()` : Afficher l'écran principal avec IP

**Pourquoi un module ?** L'écran OLED est un composant matériel réutilisable.

#### psram_info/

```
psram_info/
├── psram_info.h      ← Déclaration
└── psram_info.cpp    ← Implémentation
```

**Rôle** : Récupérer les informations sur la mémoire PSRAM de l'ESP32-S3.

**Fonctionnalités** :
- `getPsramInfo()` : Retourne une structure avec toutes les infos PSRAM

**Pourquoi un module ?** C'est une fonctionnalité autonome qui peut être utilisée partout.

---

### 📁 utils/ (utilitaires)

Les **utilitaires** sont des fichiers qui fournissent des **constantes**, **types**, ou **fonctions helpers** utilisés partout dans le projet.

#### config_constants.h

**Rôle** : Définit des constantes de configuration à partir des valeurs compilées.

**Contenu** :
```cpp
namespace ConfigConstants {
    constexpr std::string_view PROJECT_NAME    = PROJECT_NAME;
    constexpr std::string_view PROJECT_VERSION = PROJECT_VERSION;
}
```

**Pourquoi ?** Les valeurs `PROJECT_NAME` et `PROJECT_VERSION` sont définies dans `platformio.ini` et injectées à la compilation. Ce fichier les rend accessibles facilement.

#### config_state.h

**Rôle** : Stocker l'état de la configuration à l'exécution.

**Contenu** :
- Adresse IP attribuée
- SSID du WiFi connecté

**Pattern utilisé** : **Singleton** (une seule instance accessible partout)

**Pourquoi ?** Plusieurs parties du programme ont besoin de connaître l'IP et le SSID. Au lieu de les passer en paramètre partout, on utilise un état global.

#### project_info.h

**Rôle** : Fournir accès au nom et à la version du projet.

**Pourquoi un fichier séparé ?** Centralisation : si vous voulez afficher la version, vous incluez ce fichier. C'est simple et clair.

---

## 🔧 Le dossier `include/` (configuration)

### board_config.h

**Rôle** : Définit **toutes les broches GPIO** et paramètres matériels.

**Contenu** :
```cpp
#define OLED_SDA_PIN  15
#define OLED_SCL_PIN  16
#define NEOPIXEL_PIN  48
```

**Pourquoi séparer ?** Si vous changez de carte ou de câblage, vous modifiez **uniquement ce fichier**. Le reste du code n'a pas besoin de changer.

**RÈGLE IMPORTANTE** : Ne jamais modifier ce fichier sans raison valide !

### config.h

**Rôle** : Configuration logicielle (timeouts, délais, etc.).

**Contenu** :
```cpp
namespace Config {
    constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 20000;
    constexpr uint32_t WIFI_RETRY_DELAY_MS     = 500;
}
```

**Pourquoi séparer ?** Tous les paramètres de comportement sont centralisés. Facile à ajuster sans chercher dans tout le code.

### secrets.h

**Rôle** : Contient les **identifiants sensibles** (SSID WiFi, mot de passe).

**ATTENTION** : Ce fichier ne doit **JAMAIS** être partagé ou poussé sur GitHub !

---

## 📦 Le dossier `data/` (système de fichiers)

**Rôle** : Contient les fichiers web (HTML, CSS, JS) qui seront **uploadés** dans la mémoire flash de l'ESP32 (système de fichiers LittleFS).

**Fonctionnement** :
1. Les fichiers sont uploadés avec le script `auto_fs.py` ou via PlatformIO
2. L'ESP32 les lit depuis sa mémoire flash
3. Le serveur web les sert aux clients HTTP

**Pourquoi ?** L'ESP32 n'a pas de disque dur. On stocke les fichiers dans une partie de sa mémoire flash.

---

## ⚙️ platformio.ini

**Rôle** : Fichier de configuration de PlatformIO (l'environnement de développement).

**Contenu important** :
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

build_flags = 
    -D PROJECT_NAME='"Template-ESP32-S3"'
    -D PROJECT_VERSION='"1.0.0"'
```

- **build_flags** : Définit des constantes disponibles dans le code C++
- Les bibliothèques nécessaires y sont listées

**Pourquoi ici ?** Centraliser la version et le nom permet de les changer facilement pour tous les fichiers du projet.

---

## 🔗 Comment tout est relié

```
main.cpp
  │
  ├─ Include board_config.h ──────────► Définitions GPIO
  ├─ Include config.h ────────────────► Timeouts, paramètres
  │
  ├─ Créer NeoPixelStatus pixel ─────► Module LED
  ├─ Créer OledDisplay oled ─────────► Module écran
  │
  ├─ Créer WifiManager
  │    └─ Utilise pixel + oled ──────► Manager = orchestrateur
  │
  ├─ Appeler getPsramInfo() ─────────► Module info PSRAM
  │
  └─ Utiliser ConfigState::instance()► État global
```

---

## 🎓 Résumé : Pourquoi cette organisation ?

| Type | Rôle | Exemples |
|------|------|----------|
| **Modules** | Contrôler un élément matériel | LED, écran OLED, PSRAM |
| **Managers** | Orchestrer plusieurs modules | WiFi (utilise LED + écran) |
| **Utils** | Fournir des outils réutilisables | Constantes, état global |
| **Include** | Configuration matérielle/logicielle | GPIO, timeouts, secrets |

**Avantages** :
- ✅ Chaque fichier a une responsabilité claire
- ✅ Facile à tester et déboguer
- ✅ Réutilisable dans d'autres projets
- ✅ Modification d'un module n'affecte pas les autres

---

**Prochaine étape** : Lisez `02_modules_et_leur_role.md` pour comprendre en détail chaque module.
