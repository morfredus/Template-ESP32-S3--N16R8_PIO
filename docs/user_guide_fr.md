# Guide utilisateur

Version minimale valide : **1.0.2**

> 📚 **Débutant en C++ ou ESP32 ?** Consultez la [documentation didactique](/learning/README_fr.md) pour des explications détaillées !

## Fonctions principales
- Tentatives de connexion Wi-Fi avec plusieurs identifiants
- Retour OLED pendant le démarrage et la connectivité
- Indicateur d’état NeoPixel
- Interface web embarquée servie depuis LittleFS
- API d’informations de l’appareil sur `/api/info`

## Détails de l’endpoint web
- `GET /api/info`
- Retourne un JSON avec :
  - nom et version du projet
  - SSID connecté et IP locale
  - informations PSRAM (activée/taille/libre/type/mode/vitesse)
  - informations runtime carte (taille flash/heap libre/révision puce)

## Workflow de mise à jour sûr
1. Mettre à jour le code et les fichiers `data`.
2. Exécuter `pio run`.
3. Exécuter `pio run --target buildfs`.
4. Flasher le firmware puis le système de fichiers.
5. Valider la sortie série et l’endpoint web.

## Dépannage
- Si le flash fonctionne mais le contenu web est ancien, regénérer/reflasher LittleFS.
- Si le Wi-Fi échoue, vérifier les identifiants et la disponibilité réseau.
- Si l’OLED reste vide, vérifier le câblage et les broches dans `include/board_config.h`.
