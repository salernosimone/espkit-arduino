#pragma once

#include "./Setting.h"

namespace espkit::cam::sensor {
    /**
     * Enable/disable setting
     */
    class BinarySetting : public Setting {
    public:

        /**
         *
         * @param n
         * @param setter
         */
        BinarySetting(const String &n, Setter setter) : Setting(n, setter) {

        }

        /**
         * Enable setting
         */
        inline void enable() {
            set(true);
        }

        /**
         * Disable setting
         */
        inline void disable() {
            set(false);
        }

        /**
         * Set custom value
         * @param value
         */
        inline void set(bool value) {
            setter(esp_camera_sensor_get(), value ? 1.0f : 0.0f);
        }

        /**
         * Set from string
         * @param value
         */
        virtual void set(const String &value) {
            if (value == "on")
                enable();
            else if (value == "off")
                disable();
            else
                Setting::set(value);
        }
    };
}