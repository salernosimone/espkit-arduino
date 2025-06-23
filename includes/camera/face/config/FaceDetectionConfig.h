#pragma once

#include "../../../support/Confidence.h"

using espkit::support::Confidence;

namespace espkit::cam::face::config {
    /**
     * Configuration for face detection
     */
    class FaceDetectionConfig {
    public:
        Confidence confidence;
        struct {
            bool keypoints;
        } values;
        struct {
            float score;
            float nms;
            int topK;
            float resizeScale;
        } msr;
        struct {
            float score;
            float nms;
            int topK;
        } mnp;

        /**
         *
         */
        FaceDetectionConfig() {
            msr.nms = 0.5;
            msr.topK = 10;
            msr.resizeScale = 0.2;
            mnp.score = 0.5;
            mnp.nms = 0.3;
            mnp.topK = 5;

            confidence.atLeast(0.5);
            keypoints(false);
        }

        /**
         * Enable/disabled face landmarks keypoints
         * @param enabled
         */
        inline void keypoints(bool enabled = true) {
            values.keypoints = enabled;
            // as per ESP-IDF example
            msr.score = enabled ? 0.1 : 0.3;
        }
    };
}