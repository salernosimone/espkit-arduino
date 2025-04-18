#pragma once

#include "./Confidence.h"

namespace espkit::edgeimpulse::config {
    /**
     * Configuration for impulse
     */
    class ImpulseConfig {
    public:
        Confidence confidence;

        /**
         * Constructor
         */
        ImpulseConfig() {
            confidence >= 0.5;
        }
    };
}