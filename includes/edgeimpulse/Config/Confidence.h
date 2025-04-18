#pragma once

namespace espkit::edgeimpulse::config {
    /**
     * Allow for impulse.config.confidence >= 0.7 syntax
     */
    class Confidence {
    public:
        float threshold;

        /**
         * Fake comparison, it is actually a setter
         * @param thres
         * @return
         */
        bool operator>(const float thres) {
            threshold = thres + 0.0001;

            return true;
        }

        /**
         * Fake comparison, it is actually a setter
         * @param thres
         * @return
         */
        bool operator>=(const float thres) {
            threshold = thres;

            return true;
        }
    };
}