#pragma once

#include "./Config.h"

namespace esptoolkit::threads {
    /**
     * Configure thread core
     */
    class Core {
    public:
        /**
         * Constructor
         * @param c
         */
        Core(uint8_t c) : core(c) {

        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->core = core;
        }

    protected:
        uint8_t core;
    };
}