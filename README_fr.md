# Template ESP32-S3

Version minimale valide : **1.0.2**

Ce projet est un template PlatformIO pour ESP32-S3 avec :
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
