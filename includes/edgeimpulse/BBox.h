#pragma once

namespace espkit::edgeimpulse {
    /**
     * Object detection bounding box
     */
    class BBox {
    public:
        String label;
        uint8_t index;
        uint8_t x;
        uint8_t y;
        uint8_t cx;
        uint8_t cy;
        uint8_t width;
        uint8_t height;
        float confidence;

        /**
         * Constructor
         */
        BBox() {
            empty();
        }

        /**
         * Empty all data
         */
        void empty() {
            label = "";
            index = x = y = cx = cy = width = height = confidence = 0;
        }

    };
}