#pragma once

#include "./Config.h"


namespace esptoolkit::wifi::connection {
    /**
     * Configure WiFi connection retries
     */
    class Retry {
    public:

        /**
         * Default constructor
         */
        Retry() : retries(0) {

        }

        /**
         * Constructor
         * @param r
         */
        Retry(const uint8_t r) : retries(r) {

        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->retries = retries;
        }

    protected:
        uint8_t retries;
    };
}