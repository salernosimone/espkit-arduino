#pragma once

#include <model-parameters/model_metadata.h>
#include <edge-impulse-sdk/dsp/numpy_types.h>
#include <edge-impulse-sdk/dsp/image/processing.hpp>
#include <edge-impulse-sdk/classifier/ei_classifier_types.h>
#include "../support/traits/HasStatus.h"
#include "./Config/ImpulseConfig.h"

using ei::signal_t;
using espkit::support::HasStatus;
using espkit::edgeimpulse::config::ImpulseConfig;

namespace espkit::edgeimpulse {
    /**
     * Base class for specialized impulses
     */
    class Impulse : public HasStatus {
    public:
        const size_t numInputs = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
        const size_t numOutputs = EI_CLASSIFIER_LABEL_COUNT;
        signal_t signal;
        ei_impulse_result_t results;
        ImpulseConfig config;

        /**
         * Constructor
         */
        Impulse() {
            signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
        }

        /**
         * Get execution time in millis
         * @return
         */
        inline uint16_t duration() const {
            return endedAt - startedAt;
        }

        /**
         * Init impulse
         * @return
         */
        virtual Status &begin() {
            run_classifier_init();

            return succeed();
        }

    protected:
        size_t startedAt;
        size_t endedAt;

        /**
         *
         */
        inline void startBenchmark() {
            startedAt = millis();
        }

        /**
         *
         */
        inline void endBenchmark() {
            endedAt = millis();
        }
    };
}