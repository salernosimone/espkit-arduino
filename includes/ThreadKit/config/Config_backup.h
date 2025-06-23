#pragma once

#include <Arduino.h>
#include <functional>
#include "../../support/Str.h"
#include "../../support/Dict.h"

using esptoolkit::Str;
using esptoolkit::Dict;
using Task = std::function<void(void *)>;

namespace esptoolkit::internals {
    /**
     * Configuration for xCore
     */
    class ThreadConfig {
    public:
        Task task;
        String identifier;
        size_t stackSize;

        struct {
            String identifier;
            uint32_t stack;
            uint8_t priority;
            void *userdata;
            TaskHandle_t *handler;
        } values;

        /**
         * Constructor
         */
        ThreadConfig(Task t) : task(t) {
            const float maxPriority = configMAX_PRIORITIES;

            identifier(Str::random());
            stack(3000);
            priority(0);
            userdata(NULL);
            handler(NULL);

            prioritiesMap.add("lowest", 0);
            prioritiesMap.add("low", maxPriority * 0.25);
            prioritiesMap.add("base", maxPriority * 0.5);
            prioritiesMap.add("high", maxPriority * 0.75);
            prioritiesMap.add("highest", maxPriority);
        }

        /**
         * Set identifier
         * @param identifier
         */
        inline void identifier(const String &identifier) {
            values.identifier = identifier;
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