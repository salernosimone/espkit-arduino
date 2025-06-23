#pragma once

namespace esptoolkit::wifi::connection {
    /**
     * Config for WiFi connection
     */
    struct Config {
        size_t timeout;
        uint8_t retries;

        /**
         * Constructor
         */
        Config() : timeout(10000), retries(2) {

        }
    };
}