/**
* Send text to Telegram bot using httpx
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
    // gather bot and message info
    const String bot = promptString("Enter bot token: ");
    const String recipient = promptString("Enter recipient id: ");
    const String message = promptString("Enter message: ");

    // construct JSON payload for Telegram API
    JSONStr payload;

    payload.object([recipient, message](JSONStr &payload) {
        payload.attr("chat_id", recipient);
        payload.attr("text", message);
    });

    // POST request
    // skips SSL cert verification
    // JSON body and response
    auto response = httpx.post(
            String("https://api.telegram.org/bot") + bot + "/sendMessage",
            httpx.Insecure(),
            httpx.Accept("application/json"),
            httpx.ContentType("application/json"),
            httpx.Body(payload)
    );

    if (!response)
        // request failed
        ESP_LOGE("response", "fail reason: %s", response.reason().c_str());
    else
        // request was ok
        ESP_LOGI("response", "[%d] %s", response.code, response.text().c_str());

    delay(3000);
}