#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include <functional>

using Setter = std::function<void(sensor_t *s, float value)>;

namespace cam::sensor {
    /**
     *
     */
    class Setting {
    public:
        sensor_t *sensor;
        Setter setter;

        /**
         * Constructor
         * @param s
         */
        Setting(sensor_t *s, Setter set) : sensor(s), setter(set) {

        }

        /**
         * Enable setting
         */
        void enable() {
            set(true);
        }

        /**
         * Disable setting
         */
        void disable() {
            set(false);
        }

        /**
         * For negative settings (brightness/saturation)
         */
        void lowest() {
            set(-2.0f);
        }

        /**
         * For negative settings (brightness/saturation)
         */
        void low() {
            set(-1.0f);
        }

        /**
         * For negative/positive settings (brightness/saturation)
         */
        void mid() {
            set(-0.0f);
        }

        /**
         * For positive settings (brightness/saturation)
         */
        void high() {
            set(1.0f);
        }

        /**
         * For positive settings (brightness/saturation)
         */
        void highest() {
            set(2.0f);
        }

        /**
         * Set custom value
         * @param value
         */
        void set(bool value) {
            set(value ? 1.0f : 0.0f);
        }

        /**
         * Set custom value
         * @param value
         */
        void set(float value) {
            if (sensor == NULL)
                sensor = esp_camera_sensor_get();

            setter(sensor, value);
        }

        /**
         * Set from string
         * @param value
         */
        void set(const String& value) {
            if (value == "")
                return;
            else if (value == "lowest")
                lowest();
            else if (value == "low")
                low();
            else if (value == "mid")
                mid();
            else if (value == "high")
                high();
            else if (value == "highest")
                highest();
            else if (value == "on")
                enable();
            else if (value == "off")
                disable();
            else set(value.toFloat());
        }
    };
}