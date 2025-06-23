#pragma once

#include "../../../support/Str.h"

using esptoolkit::Str;

namespace esptoolkit::internals {
    /**
     * Configuration WiFiKit
     */
    class WiFiKitConfig {
    public:
        struct {
            String ssid;
            String password;
            uint32_t timeout;
            uint8_t retries;
        } values;

        /**
         * Constructor
         */
        WiFiKitConfig() {
            ssid("");
            password("");
            timeout("8 seconds");
            retries("2 times");
        }

        /**
         * Set SSID
         * @param ssid
         */
        inline void ssid(const String &ssid) {
            values.ssid = ssid;
        }

        /**
         * Set password
         * @param password
         */
        inline void password(const String &password) {
            values.password = password;
        }

        /**
         * Set timeout from String
         * @param timeout
         */
        inline void timeout(const String &timeout) {
            values.timeout = Str::from(timeout).duration();
        }

        /**
         * Set retries from String
         * @param retries
         */
        inline void retries(const String &retries) {
            values.retries = constrain(retries.toInt(), 1, 255);
        }

    };
}