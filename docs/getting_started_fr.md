# Démarrage (Débutant)

Version minimale valide : **1.0.2**

## 1) Prérequis
- VS Code
- Extension PlatformIO
- Câble USB compatible données
- Carte ESP32-S3 et pilotes adaptés

## 2) Configurer les identifiants Wi-Fi
- Ouvrir `include/secrets.h`.
- Renseigner les identifiants Wi-Fi dans la structure existante.
- Conserver le format actuel sans le modifier.

## 3) Compiler et flasher le firmware
- Compiler : `pio run`
- Flasher le firmware : `pio run --target upload`

## 4) Générer et flasher LittleFS
- Générer l’image du système de fichiers : `pio run --target buildfs`
- Flasher le système de fichiers : `pio run --target uploadfs`

## 5) Ouvrir le moniteur série
- Commande : `pio device monitor -b 115200`

## 6) Vérifier le comportement attendu
- Le NeoPixel indique l’état (scan/connexion/erreur/succès)
- L’OLED affiche le splash, la progression puis les infos Wi-Fi
- L’interface web est chargée depuis LittleFS
- L’endpoint JSON runtime est disponible sur `/api/info`
