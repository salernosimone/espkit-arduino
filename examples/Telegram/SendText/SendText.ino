/**
* Send text to Telegram bot
*/
#include <esptoolkit.h>
#include <esptoolkit/wifi.h>
#include <esptoolkit/telegram.h>


void setup() {
    delay(3000);
    Serial.begin(115200);

    // configure bot token
    String token = promptString("Enter bot token: ");
    telegramBot.configure(token);

    // connect to WiFi
    wifiConn("SSID", "PASSWORD").raise();
}

void loop() {
    // gather message info
    String recipient = promptString("Enter recipient id: ");
    String message = promptString("Enter message: ");

    telegramBot.sendText(recipient, message);
}