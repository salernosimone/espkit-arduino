#pragma once

/**
 * Ask user for string
 * @param prompt
 * @return
 */
String promptString(String prompt) {
    const size_t timeout = millis() + 8000;

    Serial.print(prompt);
    Serial.print(" ");

    while (millis() < timeout && !Serial.available())
        delay(10);

    // re-print prompt
    if (!Serial.available()) {
        Serial.println();

        return promptString(prompt);
    }

    String response = Serial.readStringUntil('\n');
    response.trim();
    Serial.println(response);

    return response;
}