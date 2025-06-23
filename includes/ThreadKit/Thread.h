#pragma once

#include <Arduino.h>
#include "../support/HasOpStatus.h"
#include "config/Config.h"
#include "./TaskWrapper.h"

using esptoolkit::internals::ThreadConfig;

namespace esptoolkit {
    /**
     * Thread function
     */
    class Thread : public HasOpStatus {
    public:
        ThreadConfig config;

        /**
         * Constructor
         * @param task
         */
        Thread(Task task) : config(task) {

        }

        /**
         * Set identifier
         * @param identifier
         * @return
         */
        Thread &withIdentifier(const String &identifier) {
            config.identifier(identifier);

            return *this;
        }

        /**
         * Set stack size
         * @param stackSize
         * @return
         */
        Thread &withStack(const String &stackSize) {
            config.stack(stackSize);

            return *this;
        }

        /**
         * Set priority
         * @param priority
         * @return
         */
        Thread &withPriority(const uint8_t priority) {
            config.priority(priority);

            return *this;
        }

        /**
         * Set priority
         * @param priority
         * @return
         */
        Thread &withPriority(const String &priority) {
            config.priority(priority);

            return *this;
        }

        /**
         * Set custom user data
         * @param userdata
         * @return
         */
        Thread &withUserData(void *userdata) {
            config.userdata(userdata);

            return *this;
        }

        /**
         * Set task handler
         * @param handler
         * @return
         */
        Thread &withHandler(TaskHandle_t *handler) {
            config.handler(handler);

            return *this;
        }

        /**
         * Attach thread to core and run
         * @param core
         * @return
         */
        OpStatus &pin(uint8_t core = 0) {
            TaskWrapper *wrapper = new TaskWrapper(
                    config.values.identifier,
                    config.task,
                    config.values.userdata
            );

            int statusCode = xTaskCreatePinnedToCore(
                    callTask,
                    config.values.identifier.c_str(),
                    config.values.stack,
//                    config.values.userdata,
                    wrapper,
                    config.values.priority,
                    config.values.handler,
                    core
            );

            if (statusCode != pdPASS)
                return failWithCode("Can't pin task", statusCode);

            return succeed();
        }

    protected:

    };
}