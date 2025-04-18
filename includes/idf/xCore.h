#pragma once

#include <Arduino.h>
#include "./xCoreConfig.h"
#include "../support/traits/HasStatus.h"

using espkit::support::HasStatus;

namespace espkit::idf {
    /**
     * Create tasks pinned to core
     */
    class xCore : HasStatus {
    public:
        xCoreConfig config;

        /**
         * Constructor
         * @param core
         */
        xCore(uint8_t core) : config(core) {

        }

        template<typename Task>
        Status &pin(const String &taskName, Task task) {
            int statusCode = xTaskCreatePinnedToCore(
                    task,
                    taskName.c_str(),
                    config.values.stack,
                    config.values.userdata,
                    config.values.priority,
                    config.values.handler,
                    config.values.core
            );

            if (statusCode != pdPASS)
                return failWithCode("Can't pin task", statusCode);

            return succeed();
        }
    };
}

// global singletons
static espkit::idf::xCore core0(0);
static espkit::idf::xCore core1(1);