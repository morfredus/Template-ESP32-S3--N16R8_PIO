# Structure du projet

Version minimale valide : **1.0.2**

## Arborescence (actuelle)
- `src/main.cpp` : point d’entrée de l’application
- `src/modules/`
  - `neopixel_status/`
  - `oled_display/`
  - `psram_info/`
- `src/managers/`
  - `wifi_manager/`
- `src/utils/`
  - headers partagés de config/état/projet
- `include/`
  - uniquement fichiers de configuration protégés
- `data/`
  - fichiers web statiques pour LittleFS
- `docs/`
  - documentation utilisateur (EN/FR)

## Règles de nommage et placement
- Classes : PascalCase
- Fonctions : camelCase
- Variables : snake_case
- Constantes/macros : UPPER_SNAKE_CASE
- Fichiers module : `module_name.h` et `module_name.cpp`
- Aucun header projet dans `include/` hors fichiers protégés

## Fichiers protégés dans include/
- `board_config.h`
- `secrets.h`
- `secrets_example.h`
- `config.h`
- `README`
