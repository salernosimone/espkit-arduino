#pragma once

#include "../../support/Dict.h"
#include "./Config.h"

using esptoolkit::Dict;

namespace esptoolkit::threads {
    /**
     * Configure thread stack size
     */
    class Priority {
    public:
        /**
         * Constructor
         * @param p
         */
        Priority(uint8_t p) : priority(p) {
        }

        /**
         *
         * @param p
         */
        Priority(const String &p) {
            const float maxPriority = configMAX_PRIORITIES;
            Dict<String, uint8_t, 5> map;

            map.add("lowest", 0);
            map.add("low", maxPriority * 0.25);
            map.add("base", maxPriority * 0.5);
            map.add("high", maxPriority * 0.75);
            map.add("highest", maxPriority);

            String str(p);

            str.toLowerCase();
            priority = map.get(str, 0);
        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->priority = priority;
        }

    protected:
        uint8_t priority;
    };
}