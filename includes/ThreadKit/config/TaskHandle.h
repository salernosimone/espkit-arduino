#pragma once

#include "./Config.h"

namespace esptoolkit::threads {
    /**
     * Configure thread TaskHandle_t
     */
    class TaskHandle {
    public:
        /**
         * Constructor
         * @param c
         */
        TaskHandle(TaskHandle_t *h) : handle(h) {

        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->handler = handle;
        }

    protected:
        TaskHandle_t *handle;
    };
}