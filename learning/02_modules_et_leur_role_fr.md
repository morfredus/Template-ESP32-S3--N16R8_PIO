# Modules et leur rôle détaillé

## 🎨 Module : NeoPixelStatus

### 📍 Localisation
```
src/modules/neopixel_status/
├── neopixel_status.h
└── neopixel_status.cpp
```

### 🎯 Rôle

Contrôler la **LED NeoPixel intégrée** de l'ESP32-S3 pour communiquer visuellement l'état du système.

### 📖 Déclaration (neopixel_status.h)

```cpp
enum class StatusColor {
    Idle,        // Inactif (blanc)
    Scanning,    // Scan WiFi en cours (jaune)
    Connecting,  // Connexion en cours (bleu)
    Connected,   // Connecté (vert)
    ErrorWifi,   // Erreur WiFi (rouge)
    ErrorApp     // Erreur application (violet)
};
```

**C'est quoi `enum class` ?**
- Une énumération : liste de valeurs possibles
- Ici, on liste tous les états possibles de la LED
- `class` signifie qu'on doit utiliser `StatusColor::Idle` (pas juste `Idle`)

```cpp
class NeoPixelStatus {
public:
    void begin();                  // Initialiser la LED
    void set(StatusColor status);  // Changer la couleur

private:
    Adafruit_NeoPixel pixel = ...;  // Objet qui contrôle la LED
};
```

### 🔧 Implémentation (neopixel_status.cpp)

```cpp
void NeoPixelStatus::begin() {
    pixel.begin();
    pixel.setBrightness(NEOPIXEL_BRIGHTNESS);
    set(StatusColor::Idle);
}
```

**Explication** :
1. Initialise le pixel NeoPixel
2. Définit la luminosité (pour ne pas aveugler !)
3. Met la LED en mode "inactif" par défaut

```cpp
void NeoPixelStatus::set(StatusColor status) {
    // Définir la couleur RGB selon l'état
    switch (status) {
        case StatusColor::Idle:       pixel.setPixelColor(0, 255, 255, 255); break; // Blanc
        case StatusColor::Scanning:   pixel.setPixelColor(0, 255, 255, 0);   break; // Jaune
        case StatusColor::Connecting: pixel.setPixelColor(0, 0, 0, 255);     break; // Bleu
        case StatusColor::Connected:  pixel.setPixelColor(0, 0, 255, 0);     break; // Vert
        case StatusColor::ErrorWifi:  pixel.setPixelColor(0, 255, 0, 0);     break; // Rouge
        case StatusColor::ErrorApp:   pixel.setPixelColor(0, 128, 0, 128);   break; // Violet
    }
    pixel.show();  // Appliquer le changement
}
```

**Explication** :
- `switch` : comme une série de `if/else` pour tester `status`
- `setPixelColor(index, R, G, B)` : définit la couleur (0-255 pour chaque canal)
- `pixel.show()` : **important** ! Sans ça, rien ne s'affiche

### 💡 Pourquoi ce module ?

Sans ce module, il faudrait écrire ce code partout où on veut changer la LED :
```cpp
pixel.setPixelColor(0, 255, 0, 0);
pixel.show();
```

Avec le module :
```cpp
pixel.set(StatusColor::ErrorWifi);  // Clair et simple !
```

**Avantages** :
- ✅ Code lisible : `StatusColor::Connected` est plus clair que `0, 255, 0`
- ✅ Centralisé : si vous voulez changer les couleurs, un seul fichier à modifier
- ✅ Réutilisable : copiez ce module dans un autre projet → ça marche

---

## 🖥️ Module : OledDisplay

### 📍 Localisation
```
src/modules/oled_display/
├── oled_display.h
└── oled_display.cpp
```

### 🎯 Rôle

Contrôler l'**écran OLED SSD1306** (128x64 pixels) pour afficher des informations locales.

### 📖 Déclaration (oled_display.h)

```cpp
class OledDisplay {
public:
    bool begin();                    // Initialiser l'écran
    void splash();                   // Écran de démarrage
    void wifiProgress(float progress); // Progression connexion (0.0 → 1.0)
    void mainScreen();               // Écran principal avec IP

private:
    Adafruit_SSD1306 display = ...;  // Objet de contrôle de l'écran
};
```

### 🔧 Implémentation (oled_display.cpp)

#### begin() - Initialisation

```cpp
bool OledDisplay::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;  // Échec
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    return true;  // Succès
}
```

**Explication** :
- `0x3C` : adresse I2C de l'écran (comme une adresse postale sur le bus I2C)
- `clearDisplay()` : efface tout
- `setTextColor(WHITE)` : texte en blanc (écran monochrome)
- `return false/true` : indique si l'initialisation a réussi

#### splash() - Écran de démarrage

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
    
    display.display();  // Afficher le contenu
}
```

**Explication** :
- `setTextSize(2)` : texte 2x plus gros
- `setCursor(x, y)` : position du texte (coin supérieur gauche)
- `print()` : écrit du texte (comme `Serial.print()`)
- `display()` : **crucial** ! Envoie le contenu à l'écran physique

**Analogie** : C'est comme écrire sur un brouillon (buffer), puis copier sur une feuille propre (`display()`).

#### wifiProgress() - Barre de progression

```cpp
void OledDisplay::wifiProgress(float progress) {
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print("Connexion WiFi...");
    
    // Dessiner une barre de progression
    int barWidth = 100;
    int filledWidth = (int)(progress * barWidth);
    
    display.drawRect(14, 30, barWidth + 2, 12, SSD1306_WHITE);
    display.fillRect(15, 31, filledWidth, 10, SSD1306_WHITE);
    
    display.display();
}
```

**Explication** :
- `progress` : valeur entre 0.0 (0%) et 1.0 (100%)
- `drawRect()` : dessine un rectangle vide (contour de la barre)
- `fillRect()` : dessine un rectangle rempli (la progression)
- `filledWidth` : calcule la largeur à remplir selon le pourcentage

**Exemple** : `progress = 0.5` → `filledWidth = 50 pixels` → barre à moitié remplie

#### mainScreen() - Écran principal

```cpp
void OledDisplay::mainScreen() {
    display.clearDisplay();
    
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Connecte :");
    
    display.setCursor(0, 15);
    display.setTextSize(2);
    display.print(ConfigState::instance().ssid().c_str());
    
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("IP:");
    
    display.setCursor(0, 50);
    display.print(ConfigState::instance().ip().c_str());
    
    display.display();
}
```

**Explication** :
- Affiche le SSID WiFi connecté
- Affiche l'adresse IP obtenue
- `.c_str()` : convertit `std::string` en chaîne C (format attendu par `print()`)

### 💡 Pourquoi ce module ?

Sans ce module, il faudrait répéter tout ce code partout. Avec le module :

```cpp
oled.splash();           // Simple !
oled.wifiProgress(0.5);  // Clair !
oled.mainScreen();       // Lisible !
```

**Avantages** :
- ✅ Encapsulation : toute la logique d'affichage est dans un seul endroit
- ✅ Maintenabilité : changer l'écran ? Modifiez seulement ce module
- ✅ Testabilité : facile à tester indépendamment

---

## 💾 Module : PsramInfo

### 📍 Localisation
```
src/modules/psram_info/
├── psram_info.h
└── psram_info.cpp
```

### 🎯 Rôle

Récupérer les **informations sur la PSRAM** (mémoire RAM externe) de l'ESP32-S3.

**C'est quoi la PSRAM ?** 
- L'ESP32-S3 a une RAM interne limitée (~512 Ko)
- La PSRAM est une RAM externe ajoutée (8 Mo sur votre carte)
- Utile pour stocker de grandes données (images, buffers, etc.)

### 📖 Déclaration (psram_info.h)

```cpp
struct PsramInfo {
    bool enabled;        // PSRAM activée ?
    size_t totalBytes;   // Taille totale en octets
    size_t freeBytes;    // Mémoire libre en octets
    std::string type;    // Type de PSRAM (ex: "OPI")
    std::string mode;    // Mode (ex: "8-line")
    std::string speed;   // Vitesse (ex: "80MHz")
};
```

**C'est quoi `struct` ?**
- Comme une `class` mais plus simple
- Regroupe plusieurs données liées
- Ici : toutes les infos PSRAM dans une structure

**C'est quoi `size_t` ?**
- Type pour représenter des tailles/quantités
- Toujours positif (pas de taille négative !)

```cpp
PsramInfo getPsramInfo();
```

**Fonction libre** (pas dans une classe) : récupère et retourne les infos.

### 🔧 Implémentation (psram_info.cpp)

```cpp
PsramInfo getPsramInfo() {
    PsramInfo info;
    
    info.enabled = psramFound();  // PSRAM détectée ?
    
    if (info.enabled) {
        info.totalBytes = ESP.getPsramSize();
        info.freeBytes = ESP.getFreePsram();
        
        // Déterminer le type selon la taille
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

**Explication** :
- `psramFound()` : fonction Arduino pour détecter la PSRAM
- `ESP.getPsramSize()` : taille totale
- `ESP.getFreePsram()` : mémoire libre disponible
- Calcul : `8 * 1024 * 1024 = 8388608 octets = 8 Mo`

### 💡 Utilisation

```cpp
PsramInfo ps = getPsramInfo();

if (ps.enabled) {
    Serial.print("PSRAM: ");
    Serial.print(ps.freeBytes);
    Serial.print(" / ");
    Serial.print(ps.totalBytes);
    Serial.println(" bytes");
}
```

**Pourquoi ce module ?**
- ✅ Encapsule la logique de récupération PSRAM
- ✅ Retourne une structure claire et complète
- ✅ Utilisé dans l'API web pour afficher les infos

---

## 📡 Manager : WifiManager

### 📍 Localisation
```
src/managers/wifi_manager/
├── wifi_manager.h
└── wifi_manager.cpp
```

### 🎯 Rôle

**Manager** (gestionnaire) qui **orchestre** la connexion WiFi en utilisant plusieurs modules.

**Pourquoi "Manager" et pas "Module" ?**
- Un **module** contrôle un élément matériel
- Un **manager** coordonne plusieurs modules pour une tâche complexe

### 📖 Déclaration (wifi_manager.h)

```cpp
class WifiManager {
public:
    WifiManager(NeoPixelStatus& px, OledDisplay& oled);
    bool connect();  // Connecter au WiFi

private:
    NeoPixelStatus& pixel;
    OledDisplay& display;
};
```

**C'est quoi `&` ?**
- Une **référence** : comme un alias ou un pointeur vers un objet existant
- Pas de copie, on utilise directement l'objet original
- Plus sûr et simple que les pointeurs

**Pourquoi passer `pixel` et `oled` ?**
- Le WifiManager a besoin d'afficher l'état sur la LED et l'écran
- Au lieu de créer ses propres instances, on utilise celles de `main.cpp`

### 🔧 Implémentation (wifi_manager.cpp)

```cpp
WifiManager::WifiManager(NeoPixelStatus& px, OledDisplay& ol)
    : pixel(px), display(ol) {
}
```

**Explication** :
- Constructeur : fonction appelée lors de la création d'un objet `WifiManager`
- `: pixel(px), display(ol)` : **liste d'initialisation** (initialise les références)

```cpp
bool WifiManager::connect() {
    pixel.set(StatusColor::Scanning);
    WiFi.mode(WIFI_STA);  // Mode station (client)
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long start = millis();  // Temps de départ
    unsigned long timeout = Config::WIFI_CONNECT_TIMEOUT_MS;
    
    while (WiFi.status() != WL_CONNECTED) {
        // Tant que pas connecté
        
        if (millis() - start > timeout) {
            // Timeout dépassé
            pixel.set(StatusColor::ErrorWifi);
            return false;  // Échec
        }
        
        // Calculer progression
        float progress = (float)(millis() - start) / timeout;
        display.wifiProgress(progress);
        
        pixel.set(StatusColor::Connecting);
        delay(Config::WIFI_RETRY_DELAY_MS);
    }
    
    // Connecté !
    pixel.set(StatusColor::Connected);
    
    // Enregistrer l'état
    ConfigState::instance().setIp(WiFi.localIP().toString().c_str());
    ConfigState::instance().setSsid(WIFI_SSID);
    
    return true;  // Succès
}
```

**Explication ligne par ligne** :

1. **Change la LED** en mode "scan"
2. **Configure WiFi** en mode station (client, pas point d'accès)
3. **Démarre la connexion** avec le SSID et mot de passe
4. **Enregistre le temps** de départ avec `millis()` (millisecondes depuis le démarrage)
5. **Boucle** tant que pas connecté
6. **Vérifie le timeout** : si trop de temps écoulé, abandon
7. **Calcule la progression** : ratio du temps écoulé / temps max
8. **Met à jour l'écran** avec la progression
9. **Change la LED** en mode "connexion"
10. **Attend** avant de réessayer
11. **Une fois connecté**, change la LED en vert
12. **Enregistre** l'IP et le SSID dans `ConfigState`
13. **Retourne `true`** pour indiquer le succès

**Pourquoi `millis()` ?**
- Retourne le nombre de millisecondes depuis le démarrage
- Permet de mesurer des durées sans bloquer le programme
- Plus précis et fiable que `delay()` pour les timeouts

### 💡 Pourquoi ce manager ?

Sans ce manager, il faudrait écrire toute cette logique dans `main.cpp` :

```cpp
// Sans WifiManager (dans main.cpp, très verbeux)
pixel.set(StatusColor::Scanning);
WiFi.mode(WIFI_STA);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
unsigned long start = millis();
while (...) {
    // tout le code de gestion
}
// etc.
```

Avec le manager :

```cpp
// Avec WifiManager (dans main.cpp, simple !)
WifiManager wifi(pixel, oled);
bool ok = wifi.connect();
```

**Avantages** :
- ✅ Séparation des responsabilités : `main.cpp` reste simple
- ✅ Testable : on peut tester la connexion WiFi indépendamment
- ✅ Réutilisable : copiez le manager dans un autre projet

---

## 📊 Récapitulatif

| Composant | Type | Responsabilité | Dépendances |
|-----------|------|----------------|-------------|
| **NeoPixelStatus** | Module | Contrôler la LED | board_config.h |
| **OledDisplay** | Module | Contrôler l'écran OLED | board_config.h, ConfigState |
| **PsramInfo** | Module | Lire infos PSRAM | ESP32 SDK |
| **WifiManager** | Manager | Orchestrer connexion WiFi | NeoPixel, OLED, Config |

**Règle d'or** :
- Un **module** = un composant matériel ou une fonction autonome
- Un **manager** = orchestrateur qui utilise plusieurs modules

---

**Prochaine étape** : Lisez `03_concepts_cpp.md` pour comprendre les concepts C++ utilisés (classes, références, namespaces, etc.).
