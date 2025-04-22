#pragma once

#include "./Setting.h"

namespace espkit::cam::sensor {
    /**
     * Brightness/Saturation setting
     */
    class RangeSetting : public Setting {
    public:

        /**
         *
         * @param n
         * @param setter
         */
        RangeSetting(const String &n, Setter setter) : Setting(n, setter) {

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
        virtual void set(float value) {
            setter(esp_camera_sensor_get(), value);
        }

        /**
         * Set from string
         * @param value
         */
        virtual void set(const String &value) {
            if (value == "lowest")
                lowest();
            else if (value == "low")
                low();
            else if (value == "mid")
                mid();
            else if (value == "high")
                high();
            else if (value == "highest")
                highest();
            else Setting::set(value);
        }
    };
}