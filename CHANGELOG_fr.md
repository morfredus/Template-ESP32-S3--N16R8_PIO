# Journal des modifications

Version minimale valide : **1.0.2**

## [1.0.2] - 2026-02-13
1. Correction du nom de fichier protégé en snake_case strict : `include/secrets_example.h`.
2. Mise à jour de toutes les références EN/FR de `secrets-example.h` vers `secrets_example.h`.
3. Suppression du dernier `Serial.println()` au démarrage pour rester conforme aux règles de logging.

## [1.0.1] - 2026-02-13
1. Réorganisation de l’architecture source pour respecter les règles strictes du projet :
   - déplacement des modules vers `src/modules/`
   - déplacement du manager vers `src/managers/`
   - déplacement des headers partagés vers `src/utils/`
2. Conservation dans `include/` uniquement des headers protégés (`board_config.h`, `secrets.h`, `secrets_example.h`, `README`).
3. Mise à jour de tous les chemins d’inclusion C++ après la réorganisation.
4. Ajout d’une documentation utilisateur débutant complète en anglais et en français dans `docs/`.
5. Ajout des fichiers de documentation racine en anglais et en français (`README*`, `CHANGELOG*`).
