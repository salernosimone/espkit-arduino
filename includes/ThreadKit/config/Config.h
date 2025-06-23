#pragma once

#include <Arduino.h>
#include <functional>
#include "../../support/Str.h"

using esptoolkit::Str;


namespace esptoolkit::threads {
    /**
     * Configuration for Thread
     */
    struct Config {
        String identifier;
        size_t stackSize;
        uint8_t priority;
        uint8_t core;
        void *userdata;
        TaskHandle_t *handler;

        /**
         * Constructor
         */
        Config() {
            identifier = Str::random();
            stackSize = 2048;
            priority = 0;
            core = 0;
            userdata = NULL;
            handler = NULL;
        }
    };
}