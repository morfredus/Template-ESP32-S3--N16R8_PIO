# Journal des modifications

Version minimale valide : **1.0.7**

## [1.0.7] - 2026-02-13
1. Ajout du préfixe "v" devant la version affichée sur l'OLED.

## [1.0.6] - 2026-02-13
1. Centrage du libellé SSID, du SSID et de la ligne IP sur l'OLED.
2. Troncature du SSID/IP pour tenir dans la largeur écran.

## [1.0.5] - 2026-02-13
1. Désactivation du retour à la ligne du SSID pour garder le libellé IP propre sur l'OLED.
2. Mise à jour de la version minimale dans la documentation.

## [1.0.4] - 2026-02-13
1. Redirection des logs verbeux vers le moniteur série en USB CDC.
2. Stockage de l'IP via un buffer formaté stable pour l'affichage OLED.

## [1.0.3] - 2026-02-13
1. Ajout de logs série verbeux pour le démarrage, le Wi-Fi, le système de fichiers et HTTP.
2. Documentation mise à jour pour les logs verbeux et la structure du projet.

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
