#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include <functional>

namespace espkit::cam::sensor {
    using Setter = std::function<void(sensor_t *s, float value)>;

    /**
     * Base class for sensor's settings
     */
    class Setting {
    public:
        const String name;
        Setter setter;

        /**
         * Constructor
         * @param s
         */
        Setting(const String& n, Setter set) : name(n), setter(set) {

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
        virtual void set(const String& value) {
            if (value != "")
                set(value.toFloat());
        }
    };
}