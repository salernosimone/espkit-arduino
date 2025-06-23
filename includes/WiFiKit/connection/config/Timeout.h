#pragma once

#include "../../../support/Str.h"
#include "./Config.h"

using esptoolkit::Str;

namespace esptoolkit::wifi::connection {
    /**
     * Configure WiFi connection timeout
     */
    class Timeout {
    public:

        /**
         * Default constructor
         */
        Timeout() : timeout(8000) {

        }

        /**
         * Constructor
         * @param t
         */
        Timeout(const size_t t) : timeout(t) {

        }

        /**
         * Constructor
         * @param t
         */
        Timeout(const String &t) {
            timeout = Str::from(t).duration();
        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->timeout = timeout;
        }

    protected:
        size_t timeout;
    };
}