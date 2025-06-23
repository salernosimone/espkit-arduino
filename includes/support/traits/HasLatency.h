#pragma once

namespace espkit::support {
    /**
     * Trait for classes that needs to benchmark execution
     */
    class HasLatency {
    public:

        /**
         * Get duration in millis
         * @return
         */
        inline size_t duration() const {
            return endedAt > startedAt ? endedAt - startedAt : 0;
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
        inline void stopBenchmark() {
            endedAt = millis();
        }
    };
}