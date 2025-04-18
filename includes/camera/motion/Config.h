#ifndef ELOQUENTESP32_INCLUDE_MOTION_CONFIG_H
#define ELOQUENTESP32_INCLUDE_MOTION_CONFIG_H

#include <Arduino.h>
#include <functional>
#include "../../../ignore/deprecated/math.h"

namespace cam::motion {
    /**
     * If true, pixel is included in count
     */
    using Include = std::function<bool(size_t x, size_t y, uint8_t pixel)>;

    /**
     * If true, pixel is considered as "changed" from old
     */
    using Compare = std::function<bool(uint8_t curr, uint8_t old)>;

    /**
     * Run function on motion detection
     */
    using OnMotion = std::function<void()>;

    /**
     * Config for motion detection
     */
    class Config {
    public:
        struct {
            uint16_t train;
            float smooth;
            float threshold;
            Include include;
            Compare compare;
//            OnMotion onMotion;
        } config;

        /**
         * Constructor
         */
        Config() {
            config.train = 10;
            config.smooth = 0.7;
            config.threshold = 0.2;
            config.include = [](uint16_t x, uint16_t y, uint8_t pixel) { return true; };
            config.compare = [](uint8_t curr, uint8_t old) { return math::absdiff(curr, old) > 10; };
        }

        /**
         * Set smooth value
         * @param s
         */
        void smooth(float s) {
            config.smooth = s;
        }

        /**
         * Set moving percent threshold
         * @param t
         */
        void threshold(float t) {
            config.threshold = t;
        }

        /**
         * Set train epochs
         * @param epochs
         */
        void train(uint16_t epochs) {
            config.train = epochs;
        }

        /**
         * Set include callback
         * @param i
         */
        void include(Include i) {
            config.include = i;
        }

        /**
         * Set compare callback
         * @param c
         */
        void compare(Compare c) {
            config.compare = c;
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_MOTION_CONFIG_H
