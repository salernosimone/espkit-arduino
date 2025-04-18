/**
* Get/Set persistent preferences
*/
#include <espkit.h>
#include <espkit/prefskit.h>

void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Preferences example");
}

void loop() {
    // you can set strings
    prefs.put("name", "Simone");
    const String name = prefs.getString("name");

    ESP_LOGI("APP", "Hello %s", name.c_str());

    // ...or numbers (always stored as float)
    prefs.put("age", 32);
    const int age = prefs.getNumber("age");

    ESP_LOGI("APP", "Age is %d", age);
    delay(3000);
}