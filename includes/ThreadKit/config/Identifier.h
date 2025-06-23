#pragma once

#include "../../support/Str.h"
#include "./Config.h"

using esptoolkit::Str;
using esptoolkit::threads::Config;

namespace esptoolkit::threads {
    /**
     * Set thread identifier
     */
    class Identifier {
    public:

        /**
         * Constructor
         * Use random identifier
         */
        Identifier() {
            identifier = Str::random();
        }

        /**
         * Constructor
         */
        Identifier(const String &n) : identifier(n) {
        }

        /**
         * 
         * @param config 
         */
        void operator()(Config *config) {
            config->identifier = identifier;
        }

    protected:
        String identifier;
    };
}