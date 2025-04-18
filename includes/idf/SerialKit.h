#pragma once

#include <Arduino.h>
#include "../support/Str.h"

using espkit::support::Str;

namespace espkit::idf {
    /**
     * Utilities to interact with Serial
     */
    class SerialKit {
    public:

        /**
         * Await for Serial to be available
         * @param duration
         * @return
         */
        bool await(const String &duration) {
            const size_t timeout = millis() + Str::from(duration).duration();

            while (millis() < timeout)
                if (Serial.available())
                    return true;

            return false;
        }

        /**
         * Syntactic sugar to read int from Serial
         * @return
         */
        int readInt() {
            return Serial.readStringUntil('\n').toInt();
        }

        /**
         * Ask user to choose from a list of options
         * @param prompt
         * @param choices
         * @param count
         * @return
         */
        String promptChoice(String prompt, String *choices, uint8_t count) {
            // prompt with separator
            Serial.println(prompt);

            for (uint8_t i = 0; i < prompt.length(); i++)
                Serial.print('=');

            Serial.println();

            // list of choices
            for (uint8_t i = 0; i < count; i++) {
                Serial.printf("[%d] %s\n", i + 1, choices[i]);
            }

            Serial.printf("Enter your choice [1-%d]: ", count);

            // parse choice until valid
            if (!await("8s")) {
                Serial.println();

                return promptChoice(prompt, choices, count);
            }

            int choice = readInt();

            Serial.println(choice);

            if (choice < 1 || choice > count) {
                ESP_LOGE("SerialKit", "Invalid choice (%d). Allowed range is 1-%d", choice, count);

                return promptChoice(prompt, choices, count);
            }

            // feedback
            ESP_LOGI("SerialKit", "Selected option %s", choices[choice - 1].c_str());

            return choices[choice - 1];
        }
    };
}

// global singleton
static espkit::idf::SerialKit serialkit;