#pragma once

#include "../../support/Str.h"
#include "./Config.h"

using esptoolkit::Str;

namespace esptoolkit::threads {
    /**
     * Configure thread stack size
     */
    class StackSize {
    public:
        /**
         * Constructor
         * @param stack
         */
        StackSize(size_t stack) : size(stack) {

        }

        /**
         * Constructor
         * @param stack
         */
        StackSize(const String &stack) {
            size = Str::from(stack).size();
        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->stackSize = size;
        }

    protected:
        size_t size;
    };
}