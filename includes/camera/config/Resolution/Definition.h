#pragma once

#include <esp_camera.h>

namespace espkit::cam::resolution {
    /**
     * Resolution definition
     */
    class Definition {
    public:
        String name;
        framesize_t size;
        uint16_t width;
        uint16_t height;

        /**
         * Constructor
         * @param n
         * @param s
         * @param w
         * @param h
         */
        Definition(const String &n, framesize_t s, uint16_t w, uint16_t h) :
                name(n),
                size(s),
                width(w),
                height(h),
                repr(String(w) + " x " + String(h)) {

        }

        /**
         * Test if model matches by name or dimensions
         * @param n
         * @return
         */
        inline bool matches(const String &n) const {
            return n == name || n == repr;
        }

        /**
         * Get width x height representation
         * @return
         */
        inline const String &dimensions() const {
            return repr;
        }

    protected:
        const String repr;
    };
}