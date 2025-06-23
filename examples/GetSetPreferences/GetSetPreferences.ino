/**
* Get/Set persistent preferences
*/
#include <esptoolkit.h>
#include <esptoolkit/preferences.h>

void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Preferences example");
}

void loop() {
    // you can set strings
    prefsKit.put("name", "Simone");
    const String name = prefsKit.getString("name");

    ESP_LOGI("APP", "Hello %s", name.c_str());

    // ...or numbers (always stored as float)
    prefsKit.put("age", 32);
    const int age = prefsKit.getNumber("age");

    ESP_LOGI("APP", "Age is %d", age);
    delay(3000);
}