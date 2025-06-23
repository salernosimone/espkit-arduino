/**
* Do POST requests with httpx
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
    // POST request
    // skips SSL cert verification
    // "Accept: text/plain" header
    // "Referer" header
    // connection and request timeout
    auto response = httpx.post(
            "https://httpbin.org/post",
            httpx.Insecure(),
            httpx.Accept("application/json"),
            httpx.ContentType("application/json"),
            httpx.Header("X-Custom-Header", "esptoolkit"),
            httpx.Body("[1, 2, 3]")
    );

    if (!response)
        // request failed
        ESP_LOGE("response", "fail reason: %s", response.reason().c_str());
    else
        // request was ok
        ESP_LOGI("response", "%d: %s", response.code, response.text().c_str());

    delay(3000);
}