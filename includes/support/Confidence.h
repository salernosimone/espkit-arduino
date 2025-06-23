#pragma once

namespace espkit::support {
    /**
     * Allow for *.config.confidence >= 0.7 syntax
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
            atLeast(thres);

            return true;
        }

        /**
         * Fake comparison, it is actually a setter
         * @param thres
         * @return
         */
        bool operator>=(const float thres) {
            atLeast(thres);

            return true;
        }

        /**
         * Set threshold
         * @param thres
         */
        inline void atLeast(const float thres) {
            threshold = thres;
        }
    };
}