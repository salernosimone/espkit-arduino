#pragma once

#include <esp_camera.h>

namespace espkit::cam::pinout {
    /**
     * Pinout definition
     */
    class Definition {
    public:
        const String name;
        const uint8_t pins[16];

        /**
         *
         * @param n
         * @param d0
         */
        Definition(const String &n, const uint8_t d0, const uint8_t d1, const uint8_t d2, const uint8_t d3,
                   const uint8_t d4, const uint8_t d5, const uint8_t d6, const uint8_t d7, const uint8_t xclk,
                   const uint8_t pclk, const uint8_t vsync, const uint8_t href, const uint8_t sda, const uint8_t scl,
                   const uint8_t pwdn, const uint8_t reset) :
                name(n), pins{d0, d1, d2, d3, d4, d5, d6, d7, xclk, pclk, vsync, href, sda, scl, pwdn, reset} {
        }

        /**
         * Test if pinout name matches given name
         * @param n
         * @return
         */
        inline bool matches(const String &n) const {
            return name == n;
        }

        /**
         * Set pins on config instance
         * @param config
         */
        void applyTo(camera_config_t *config) const {
            config->pin_d0 = pins[0];
            config->pin_d1 = pins[1];
            config->pin_d2 = pins[2];
            config->pin_d3 = pins[3];
            config->pin_d4 = pins[4];
            config->pin_d5 = pins[5];
            config->pin_d6 = pins[6];
            config->pin_d7 = pins[7];
            config->pin_xclk = pins[8];
            config->pin_pclk = pins[9];
            config->pin_vsync = pins[10];
            config->pin_href = pins[11];
            config->pin_sccb_sda = pins[12];
            config->pin_sccb_scl = pins[13];
            config->pin_pwdn = pins[14];
            config->pin_reset = pins[15];
        }
    };
}