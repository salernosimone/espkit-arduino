/**
 * CameraWebServer example with a new style
 *
 * Set Tools > Core Debug Level > Info
 */
#include <espkit.h>
#include <espkit/wifikit.h>
#include <espkit/mdnskit.h>
#include <espkit/camera-webserver.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("App", "CameraWebServer example");

    // try to connect to WiFi
    // default timeout is 8 seconds
    // default retries is 2
    wifikit.to("SSID", "PASSWORD").connect();
    wifikit.raise();

    ESP_LOGI("APP", "Connected to WiFi with IP %s", wifikit.ip.c_str());

    // set hostname
    // camera will be accessible at http://esp32cam.local
    mdnskit.begin("esp32cam");

    // configure the camera
    camera.config.pinout.prompt();
    camera.config.pixformat.jpeg();
    camera.config.quality.high();
    camera.config.resolution.vga();

    // init camera and raise error if something goes wrong
    camera.begin();
    camera.raise();

    // init server
    cameraWebServer.listenOn(80);
    cameraWebServer.begin();
}

void loop() {
    // Camera HTTP server runs in a background thread
}