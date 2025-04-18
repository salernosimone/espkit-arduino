/**
* Run a function only once, even if called inside loop()
*/
#include <espkit.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Once example");
}

void loop() {
    // you can call a function once even if it's inside loop()
    // because why not?
    once([]{
        ESP_LOGI("APP", "Called once");
    });

    // of course you can define more than one function
    once([]{
        ESP_LOGI("APP", "Called another once");
    });
}