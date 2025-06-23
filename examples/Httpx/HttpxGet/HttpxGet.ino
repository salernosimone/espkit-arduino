/**
* Do GET requests with httpx
*/
#include <esptoolkit.h>
#include <esptoolkit/wifi.h>
#include <esptoolkit/httpx.h>


void setup() {
    delay(3000);
    Serial.begin(115200);

    // connect to WiFi
    wifiConn("SSID", "PASSWORD").raise();
}

void loop() {
    // GET request
    // skips SSL cert verification
    // "Accept: text/plain" header
    // "Referer" header
    // connection and request timeout
    auto response = httpx.get(
            "https://icanhazdadjoke.com/",
            httpx.Insecure(),
            httpx.Accept("text/plain"),
            httpx.Header("Referer", "arduino/esptoolkit"),
            httpx.ConnectTimeout("2s"),
            httpx.RequestTimeout("6s")
    );

    if (!response)
        // request failed
        ESP_LOGE("response", "%s", response.reason().c_str());
    else
        // request was ok
        ESP_LOGI("response", "%d: %s", response.code, response.text().c_str());

    delay(3000);
}