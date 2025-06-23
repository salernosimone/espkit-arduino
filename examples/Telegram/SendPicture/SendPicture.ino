/**
* Send picture to Telegram bot
*/
#include <esptoolkit.h>
#include <esptoolkit/wifi.h>
#include <esptoolkit/telegram.h>
#include "./image.h"


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
    String caption = promptString("Enter caption (or leave empty): ");

    // send picture
    telegramBot.withCaption(caption).sendPicture(recipient, image, sizeof(image));

    // if something went wrong, log message
    if (!telegramBot)
        ESP_LOGE("Telegram", "ERROR: %s", telegramBot.reason().c_str());

    delay(5000);
}