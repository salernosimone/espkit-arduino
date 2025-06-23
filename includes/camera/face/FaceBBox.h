#pragma once

#include "../../support/traits/JSONable.h"
#include "../../support/JSONStr.h"
#include "../../support/Str.h"
#include "./Landmarks.h"

using espkit::support::JSONStr;
using espkit::support::JSONable;
using espkit::support::Str;

namespace espkit::cam::face {
    /**
     * Face bounding box
     */
    class FaceBBox : public JSONable {
    public:
        uint8_t index;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        uint16_t cx;
        uint16_t cy;
        float confidence;
        Landmarks landmarks;

        /**
         *
         */
        FaceBBox() {
            empty();
            landmarks.empty();
        }

        /**
         *
         */
        void empty() {
            x = y = width = height = cx = cy = confidence = 0;
        }

        /**
         *
         * @param box
         */
        void setCoordinates(std::vector<int> &box) {
            x = box[0];
            y = box[1];
            width = box[2];
            height = box[3];
            cx = x + width / 2;
            cy = y + height / 2;
        }

        /**
         *
         * @return
         */
        String &toJSON() {
            Str bbox = Str::from("{").concat("\"x\"", x, ",\"y\":", y, ",\"width\":", width, ",\"height\":", height,
                                             ",\"confidence\":", String(confidence, 2), "}");
            json.reserve(300);
            json.openObject();
            json.addRawProp("bbox", bbox.toString());
            if (landmarks) json.addRawProp("landmarks", landmarks.toJSON());
            json.closeObject();

            return json.toString();
        }

    protected:
        JSONStr json;
    };
}