# Introduction au C++ et à ce projet ESP32

## 📘 Qu'est-ce que le C++ ?

Le **C++** est un langage de programmation compilé, c'est-à-dire que votre code est transformé en instructions machine avant d'être exécuté. C'est différent des langages interprétés comme Python.

### Pourquoi le C++ pour l'ESP32 ?

1. **Performance** : Le C++ est très rapide car il communique directement avec le matériel
2. **Contrôle** : Vous gérez précisément la mémoire et les ressources
3. **Arduino Framework** : Une couche simplifiée qui rend le C++ plus accessible pour les microcontrôleurs

## 🎯 Ce que fait ce projet

Ce projet est un **template** (modèle) pour démarrer rapidement avec un ESP32-S3. Il inclut :

- ✅ Connexion WiFi automatique
- ✅ Serveur web pour afficher des informations
- ✅ Écran OLED pour l'affichage local
- ✅ LED NeoPixel pour indiquer l'état
- ✅ Gestion de la mémoire PSRAM

## 📁 Concepts de base à connaître

### 1. Les fichiers `.h` (headers)

Les fichiers `.h` sont comme des **sommaires** ou des **tables des matières**. Ils déclarent ce qui existe (fonctions, classes, constantes) mais ne contiennent généralement pas l'implémentation complète.

**Pourquoi ?** Cela permet à d'autres fichiers de savoir ce qui est disponible sans avoir besoin de tout le code.

### 2. Les fichiers `.cpp` (source)

Les fichiers `.cpp` contiennent l'**implémentation réelle** du code, c'est-à-dire comment les choses fonctionnent.

### 3. L'include (#include)

```cpp
#include "wifi_manager.h"
```

Cette ligne dit : "Je vais utiliser des choses définies dans wifi_manager.h, va chercher ce fichier et intègre-le ici."

### 4. Les classes

Une **classe** est un modèle pour créer des objets. C'est comme un plan de construction.

```cpp
class WifiManager {
    // Déclaration : ce que peut faire un WifiManager
};
```

### 5. Les namespaces

Un **namespace** est comme un dossier pour organiser le code et éviter les conflits de noms.

```cpp
namespace Config {
    constexpr uint32_t WIFI_TIMEOUT = 20000;
}
// Pour l'utiliser : Config::WIFI_TIMEOUT
```

## 🔄 Le cycle de vie d'un programme ESP32

```
┌─────────────────┐
│   Démarrage     │
│   (power on)    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   setup()       │ ← Exécuté UNE SEULE FOIS au démarrage
│                 │   (initialisation du WiFi, écran, etc.)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   loop()        │ ← Exécuté EN BOUCLE indéfiniment
│                 │   (gestion des événements, mise à jour)
└────────┬────────┘
         │
         └──────────► Retour au début de loop()
```

**Important** : Dans ce projet, `loop()` est vide car nous utilisons un serveur web asynchrone qui gère les requêtes automatiquement !

## 🧩 Organisation du code

### Approche modulaire

Au lieu de mettre tout le code dans un seul gros fichier, nous le **découpons en modules** :

- Chaque module a une responsabilité claire
- Les modules sont réutilisables
- Le code est plus facile à comprendre et maintenir

**Analogie** : C'est comme construire une maison avec des pièces séparées (cuisine, chambre, salon) plutôt qu'un grand espace ouvert sans murs.

## 📚 Structure de ce guide

1. **00_introduction_cpp.md** ← Vous êtes ici
2. **01_structure_du_projet.md** - Organisation des dossiers
3. **02_modules_et_leur_role.md** - Détail de chaque module
4. **03_concepts_cpp.md** - Les concepts C++ utilisés
5. **04_main_explique.md** - Analyse ligne par ligne du programme principal
6. **05_configuration.md** - Les fichiers de configuration

---

**Prochaine étape** : Lisez `01_structure_du_projet.md` pour comprendre comment le projet est organisé.
