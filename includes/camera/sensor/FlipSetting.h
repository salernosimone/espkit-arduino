#pragma once

#include "./Setting.h"

namespace espkit::cam::sensor {
    /**
     * Vertical/Horizontal flip
     */
    class FlipSetting : public Setting {
    public:

        /**
         *
         * @param n
         * @param setter
         */
        FlipSetting() : Setting("flip", [](sensor_t *s, float x) {}) {

        }

        /**
         * No flip
         */
        inline void none() {
            set(0);
        }

        /**
         * Vertical flip
         */
        inline void vertical() {
            set(1);
        }

        /**
         * Horizontal flip (a.k.a. mirror)
         */
        inline void horizontal() {
            set(2);
        }

        /**
         * Vertical & Horizontal
         */
        inline void both() {
            set(3);
        }

        /**
         * Set custom value
         * @param value
         */
        inline void set(float value) {
            uint8_t v = value;
            sensor_t * s = esp_camera_sensor_get();

            s->set_vflip(s, v & 0x1);
            s->set_hmirror(s, v & 0x2);
        }

        /**
         * Set from string
         * @param value
         */
        virtual void set(const String &value) {
            if (value.startsWith("v"))
                vertical();
            else if (value.startsWith("h"))
                horizontal();
            else if (value.startsWith("b"))
                both();
            else if (value.startsWith("n"))
                none();
            else
                Setting::set(value);
        }
    };
}