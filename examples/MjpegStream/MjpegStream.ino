/**
 * See camera stream in MJPEG format.
 * Useful to capture images, too.
 *
 * Set Tools > Core Debug Level > Info
 */
#include <espkit.h>
#include <espkit/wifikit.h>
#include <espkit/mdnskit.h>
#include <espkit/camera-mjpeg.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("App", "Mjpeg Stream example");

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
    camera.config.pinout.wroom_s3();
    camera.config.pixformat.jpeg();
    camera.config.quality.high();
    camera.config.resolution.square();

    // init camera and raise error if something goes wrong
    camera.begin();
    camera.raise();

    // init server
    mjpegServer.listenOn(80);
    mjpegServer.begin();
    ESP_LOGI("APP", "Server is running...");
}

void loop() {
    // MJPEG HTTP server runs in a background thread
}