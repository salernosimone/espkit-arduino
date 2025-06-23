#pragma once

#include <Arduino.h>
#include "../../support/traits/JSONable.h"
#include "../../support/JSONStr.h"

using espkit::support::JSONable;
using espkit::support::JSONStr;

namespace espkit::cam::face {
    /**
     * Face detection keypoint
     */
    class Keypoint : public JSONable {
    public:
        uint16_t x;
        uint16_t y;

        /**
         *
         */
        inline void empty() {
            x = y = 0;
        }

        /**
         * Update coords
         * @param x_
         * @param y_
         */
        void set(uint16_t x_, uint16_t y_) {
            x = x_;
            y = y_;
        }

        /**
         * Convert to JSON
         * @return
         */
        String &toJSON() {
            json.reserve(20);

            json.openObject();
            json.addProp("x", x);
            json.addProp("y", y);
            json.closeObject();

            return json.toString();
        }

    protected:
        JSONStr json;
    };
}