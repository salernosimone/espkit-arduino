/**
 * Utilities to connect to a WiFi network
 *
 * Set Tools > Core Debug Level > Info
 */
#include <esptoolkit.h>
#include <esptoolkit/wifi.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Connect to WiFi example");

    // try to connect
    // default timeout is 8 seconds
    // default retries is 0
    wifiConn("SSID", "PASSWORD");

    if (wifiConn) {
        // we are connected to WiFi, print IP address
        ESP_LOGI("APP", "Connected to WiFi with IP %s", wifiConn.ip.c_str());
    } else {
        // connection didn't succeded
        ESP_LOGE("APP", "Can't connect to WiFi");
    }

    // you can configure the connection timeout and retries
    // since these arguments are optional, you use
    // named arguments (you can re-order them as you like).
    // I vastly prefer this style instead of wifiConn("SSID", "PASS", "8s", 2)
    // "What's 8s? What does 2 stand for?"
    // if only C++ had named arguments like Python...
    wifiConn("SSID", "PASSWORD", wifiConn.Timeout("8s"), wifiConn.Retry(2));

    // you can abort if connection was not successful
    wifiConn("BAD SSID", "BAD PASSWORD").raise();
}

void loop() {
}