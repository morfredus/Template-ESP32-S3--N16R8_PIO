#include "utils/logging.h"

void initLogging() {
    esp_log_level_set("*", ESP_LOG_DEBUG);
}
