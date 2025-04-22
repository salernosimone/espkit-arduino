#pragma once

#include "./Setting.h"

namespace espkit::cam::sensor {
    /**
     * Set sensor special effect
     */
    class SpecialEffect : public Setting {
    public:

        /**
         * Constructor
         * @param s
         */
        SpecialEffect() : Setting("effect", [](sensor_t *s, float x) { s->set_special_effect(s, x); }) {

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
        virtual void set(float effect) {
            setter(esp_camera_sensor_get(), effect);
        }

        /**
         * Set special effect from value
         * @param effect
         */
        void set(const String &effect) {
            static String effects[] = {"none", "negative", "gray", "red", "green", "blue", "sepia"};

            if (effect == "") {
                set(0);
                return;
            }

            for (uint8_t i = 0; i < 7; i++) {
                if (effect.startsWith(effects[i])) {
                    set(i);
                    return;
                }
            }

            ESP_LOGE("Camera.Sensor", "Unknown special effect: %s", effect.c_str());
        }
    };
}