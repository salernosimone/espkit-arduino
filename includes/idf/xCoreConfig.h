#pragma once

#include <Arduino.h>
#include "../support/Str.h"
#include "../support/Dict.h"

using espkit::support::Str;
using espkit::support::Dict;

namespace espkit::idf {
    /**
     * Configuration for xCore
     */
    class xCoreConfig {
    public:
        struct {
            uint8_t core;
            uint32_t stack;
            uint8_t priority;
            void *userdata;
            TaskHandle_t *handler;
        } values;

        /**
         * Constructor
         */
        xCoreConfig(uint8_t core) {
            const float maxPriority = configMAX_PRIORITIES;

            values.core = core;
            stack(3000);
            priority(0);
            userdata(NULL);
            handler(NULL);

            prioritiesMap.add("lowest", 0);
            prioritiesMap.add("low", maxPriority * 0.25);
            prioritiesMap.add("mid", maxPriority * 0.5);
            prioritiesMap.add("high", maxPriority * 0.75);
            prioritiesMap.add("highest", maxPriority);
        }

        /**
         * Set stack size from int
         * @param stack
         */
        inline void stack(const uint32_t stack) {
            values.stack = stack;
        }

        /**
         * Set stack size from String
         * @param stack
         */
        inline void stack(const String &stack) {
            values.stack = Str::from(stack).size();
        }

        /**
         * Set priority from int
         * @param priority
         */
        inline void priority(const uint8_t priority) {
            values.priority = priority;
        }

        /**
         * Set priority from String
         * @param priority
         */
        inline void priority(const String &priority) {
            String str(priority);

            str.toLowerCase();
            values.priority = prioritiesMap.get(priority, 0);
        }

        /**
         * Set userdata pointer
         * @param userdata
         */
        inline void userdata(void *userdata) {
            values.userdata = userdata;
        }

        /**
         * Set task handle to keep track of task
         * @param handler
         */
        inline void handler(TaskHandle_t *handler) {
            values.handler = handler;
        }

    protected:
        Dict<String, uint8_t, 5> prioritiesMap;
    };
}