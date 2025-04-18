/**
 * Utilities to connect to a WiFi network
 *
 * Set Tools > Core Debug Level > Info
 */
#include <espkit.h>
#include <espkit/wifikit.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Connect to WiFi example");

    // try to connect
    // default timeout is 8 seconds
    // default retries is 2
    wifikit.to("SSID", "PASSWORD").connect();

    if (wifikit) {
        // we are connected to WiFi, print IP address
        ESP_LOGI("APP", "Connected to WiFi with IP %s", wifikit.ip.c_str());
    } else {
        // connection didn't succeded
        ESP_LOGE("APP", "Can't connect to WiFi");
    }

    // you can configure the connection timeout and retries
    // (before you try to connect!)
    wifikit.config.timeout("10 seconds");
    wifikit.config.retries("3 times");
}

void loop() {
}