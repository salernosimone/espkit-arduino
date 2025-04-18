#pragma once

#include <esp_camera.h>

namespace cam::sensor {
    /**
     * Set sensor special effect
     */
    class SpecialEffect {
    public:
        sensor_t *sensor;

        /**
         * Constructor
         * @param s
         */
        SpecialEffect(sensor_t *s) : sensor(s) {

        }

        /**
         *
         */
        inline void none() {
            set(0);
        }

        /**
         *
         */
        inline void negative() {
            set(1);
        }

        /**
         *
         */
        inline void gray() {
            set(2);
        }

        /**
         *
         */
        inline void redTint() {
            set(3);
        }

        /**
         *
         */
        inline void greenTint() {
            set(4);
        }

        /**
         *
         */
        inline void blueTint() {
            set(5);
        }

        /**
         *
         */
        inline void sepia() {
            set(6);
        }

        /**
         * Set special effect from value
         * @param effect
         */
        void set(uint8_t effect) {
            sensor_t *sensor = esp_camera_sensor_get();
            sensor->set_special_effect(sensor, effect);
        }

        /**
         * Set special effect from value
         * @param effect
         */
        void set(String effect) {
            static String effects[] = {"none", "negative", "gray", "red", "green", "blue", "sepia"};

            for (uint8_t i = 0; i < 7; i++) {
                if (effects[i] == effect) {
                    set(i);
                    return;
                }
            }

            ESP_LOGE("Camera.Sensor", "Unknown special effect: %s", effect.c_str());
        }
    };
}