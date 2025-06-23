#pragma once

#include "../../support/Confidence.h"

using espkit::support::Confidence;

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