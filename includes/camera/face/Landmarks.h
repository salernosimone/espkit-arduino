#pragma once

#include <Arduino.h>
#include "../../support/traits/JSONable.h"
#include "../../support/JSONStr.h"
#include "./Keypoint.h"

using espkit::support::JSONStr;
using espkit::support::JSONable;

namespace espkit::cam::face {
    /**
     * Face landmarks
     */
    class Landmarks : public JSONable {
    public:
        Keypoint nose;
        struct {
            Keypoint left;
            Keypoint right;
        } eyes, mouth;

        /**
         * Test if landmarks are valid
         * @return
         */
        explicit operator bool() const {
            return eyes.right.x > eyes.left.x;
        }

        /**
         * Reset landmarks
         */
        void empty() {
            nose.empty();
            eyes.left.empty();
            eyes.right.empty();
            mouth.left.empty();
            mouth.right.empty();
        }

        /**
         * Set keypoints coordinates
         * @param kp
         */
        void set(std::vector<int> kp) {
            eyes.left.set(kp[0], kp[1]);
            eyes.right.set(kp[6], kp[7]);
            nose.set(kp[4], kp[5]);
            mouth.left.set(kp[2], kp[3]);
            mouth.right.set(kp[8], kp[9]);
        }

        /**
         * Convert to JSON
         * @return
         */
        String &toJSON() {
            json.reserve(130);
            json.openObject();
            json.addRawProp("le", eyes.left.toJSON());
            json.addRawProp("re", eyes.right.toJSON());
            json.addRawProp("nose", nose.toJSON());
            json.addRawProp("lm", mouth.left.toJSON());
            json.addRawProp("rm", mouth.right.toJSON());
            json.closeObject();

            return json.toString();
        }

    protected:
        JSONStr json;
    };
}