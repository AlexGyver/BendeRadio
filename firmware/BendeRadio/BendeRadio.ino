#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "core0.h"

TaskHandle_t Task0;

void setup() {
    xTaskCreatePinnedToCore(core0, "Task0", 10000, NULL, 1, &Task0, 0);

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        anim_search();
    }
    Serial.println();
    Serial.println(WiFi.localIP());
    change_state();
}

void loop() {
    audio.loop();

    if (reconnect) {
        audio.connecttohost(reconnect);
        if (!audio.isRunning()) audio.pauseResume();
        reconnect = nullptr;
    }
}