#pragma once

#include "./Setting.h"

namespace espkit::cam::sensor {
    /**
     * Setting in a custom range.
     * If larger than 0-100, map to 0-100
     */
    class BoundedSetting : public Setting {
    public:

        /**
         *
         * @param n
         * @param l
         * @param h
         * @param setter
         */
        BoundedSetting(const String &n, const float h, Setter setter) :
                Setting(n, setter),
                high(h) {

        }

        /**
         * Set custom value
         * @param value
         */
        virtual void set(float value) {
            if (value > high) {
                ESP_LOGE("Camera.Sensor.Setting", "%s value must be in range 0 - %d (%d given)", name.c_str(),
                         (int) high, (int) value);
                return;
            }

            // map from 0-100 to 0-high
            if (high > 100)
                value = (value * high) / 100;

            setter(esp_camera_sensor_get(), value);
        }

        /**
         * Set from string
         * @param value
         */
        virtual void set(const String &value) {
            if (value != "")
                set(value.toFloat());
        }

    protected:
        const float high;
    };
}