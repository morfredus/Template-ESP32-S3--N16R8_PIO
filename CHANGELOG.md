# Changelog

Minimum valid version: **1.0.3**

## [1.0.3] - 2026-02-13
1. Added verbose serial logging for startup, Wi-Fi, filesystem, and HTTP.
2. Documented verbose logs and updated project structure references.

## [1.0.2] - 2026-02-13
1. Fixed protected filename to strict snake_case: `include/secrets_example.h`.
2. Updated all EN/FR documentation references from `secrets-example.h` to `secrets_example.h`.
3. Removed remaining `Serial.println()` usage in startup path to keep logging rules compliant.

## [1.0.1] - 2026-02-13
1. Reorganized source architecture to follow strict project rules:
   - moved modules to `src/modules/`
   - moved manager to `src/managers/`
   - moved shared headers to `src/utils/`
2. Kept protected headers in `include/` only (`board_config.h`, `secrets.h`, `secrets_example.h`, `README`).
3. Updated all C++ include paths after the reorganization.
4. Added complete beginner user documentation in English and French under `docs/`.
5. Added root documentation files in English and French (`README*`, `CHANGELOG*`).
