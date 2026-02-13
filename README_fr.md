# Template ESP32-S3 - Projet Pédagogique pour Débutants

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange.svg)](https://platformio.org/)
[![Beginner Friendly](https://img.shields.io/badge/Débutant-Friendly-green.svg)](learning/README.md)
[![Documentation](https://img.shields.io/badge/Documentation-Complète-blue.svg)](learning/README.md)
[![C++](https://img.shields.io/badge/C%2B%2B-Explained-red.svg)](learning/03_concepts_cpp.md)

> 🎓 **Template complet avec documentation didactique ligne par ligne**  
> Parfait pour apprendre le C++, PlatformIO et le développement embarqué sur ESP32

Version minimale valide : **1.0.2**

## 🚀 Qu'est-ce que c'est ?

Un template PlatformIO **prêt à l'emploi** pour ESP32-S3 avec **documentation complète pour débutants**. Idéal pour :
- 📚 **Apprendre le C++** et le développement embarqué
- 🎯 **Comprendre** comment fonctionne un projet ESP32 professionnel
- 🛠️ **Démarrer rapidement** vos propres projets IoT

## ✨ Fonctionnalités incluses
- Connexion Wi-Fi via plusieurs identifiants dans `include/secrets.h`
- Écrans d’état sur OLED
- Retour visuel via le NeoPixel intégré
- Serveur web statique LittleFS (`data/index.html`, `data/style.css`)
- API d’informations runtime sur `/api/info`

## Démarrage rapide (débutant)
1. Installer VS Code, l’extension PlatformIO et les pilotes USB de votre carte ESP32-S3.
2. Copier `include/secrets_example.h` vers votre configuration locale si nécessaire et conserver `include/secrets.h` conforme au format attendu.
3. Connecter la carte en USB.
4. Compiler le firmware : `pio run`
5. Flasher le firmware : `pio run --target upload`
6. Générer l’image du système de fichiers : `pio run --target buildfs`
7. Flasher le système de fichiers : `pio run --target uploadfs`
8. Ouvrir le moniteur série : `pio device monitor -b 115200`

## Structure du projet
- `src/main.cpp` : point d’entrée de l’application
- `src/modules/` : modules matériels/fonctionnels
- `src/managers/` : gestionnaires d’orchestration
- `src/utils/` : headers utilitaires/configuration partagés
- `include/` : uniquement headers de configuration protégés
- `docs/` : documentation utilisateur (EN/FR)

## Documentation
- Guide débutant : `docs/getting_started_fr.md`
- Guide utilisateur : `docs/user_guide_fr.md`
- Architecture et arborescence : `docs/project_structure_fr.md`
- Versions anglaises disponibles sans suffixe `_fr`.

### 📚 Documentation didactique (apprentissage C++)

Pour les débutants souhaitant **comprendre en profondeur** le fonctionnement du code et les concepts C++, une documentation complète est disponible dans le dossier `learning/` :

- **Guide complet** : [`learning/README.md`](learning/README.md) - Point d'entrée avec navigation
- Introduction au C++ : [`learning/00_introduction_cpp.md`](learning/00_introduction_cpp.md)
- Structure du projet : [`learning/01_structure_du_projet.md`](learning/01_structure_du_projet.md)
- Modules détaillés : [`learning/02_modules_et_leur_role.md`](learning/02_modules_et_leur_role.md)
- Concepts C++ : [`learning/03_concepts_cpp.md`](learning/03_concepts_cpp.md)
- Analyse de main.cpp : [`learning/04_main_explique.md`](learning/04_main_explique.md)
- Configuration : [`learning/05_configuration.md`](learning/05_configuration.md)

Cette documentation explique **ligne par ligne** le code, les choix techniques, et les concepts utilisés, avec des analogies et exemples concrets.
