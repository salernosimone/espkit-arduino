#pragma once

#include <Arduino.h>
#include "../support/Str.h"
#include "../support/HasOpStatus.h"

using espkit::support::Str;
using espkit::support::Status;
using espkit::support::HasStatus;

namespace espkit::idf {
    /**
     * A simple semaphore
     */
    class xLock : public HasStatus {
    public:
        const char *name;
        SemaphoreHandle_t lock;

        /**
         * Constructor
         * @param name_
         */
        xLock(const char *name_) :
                name(name_),
                lock(NULL) {

            block();
        }

        /**
         * Block until the lock is acquired
         * @return
         */
        inline xLock &block() {
            return await("0s");
        }

        /**
         * Await for given millis to get the lock
         * @param timeout
         * @return
         */
        inline xLock &await(const String &timeout) {
            config.timeout = Str::from(timeout).duration();

            return *this;
        }

        /**
         * Acquire lock
         * @return
         */
        Status &acquire() {
            const portTickType ticks = config.timeout ? config.timeout / portTICK_RATE_MS : portMAX_DELAY;

            if (!create())
                return status;

            if (xSemaphoreTake(lock, ticks) != pdTRUE)
                return fail(Str::from("Can't acquire lock ").concat(name, " within ", config.timeout, " ms"));

            return succeed();
        }

        /**
         * Release lock
         */
        void release() {
            if (lock != NULL)
                xSemaphoreGive(lock);
        }

        /**
         * Run callback within a locked session
         * @tparam Callback
         * @param callback
         * @return
         */
        template<typename Callback>
        Status &isolated(Callback callback) {
            if (!create())
                return status;

            if (!acquire())
                return status;

            callback();
            release();

            return succeed();
        }

    protected:
        struct {
            size_t timeout;
        } config;

        /**
         * Instantiate lock if required
         * @return
         */
        Status &create() {
            if (lock == NULL)
                lock = xSemaphoreCreateMutex();

            if (lock == NULL)
                return fail(Str::from("Can't create ").concat(name, " lock"));

            return succeed();
        }
    };
}