#pragma once

#include "../functions/swapBytes.h"

namespace espkit::video {
    /**
     * A basic frame (data + dimensions)
     */
    class Frame {
    public:
        uint16_t width;
        uint16_t height;

        /**
         * Check if frame is empty
         * @return
         */
        virtual operator bool() const {
            return size() > 0;
        }

        /**
         * Get size in bytes
         * @return
         */
        inline size_t size() const {
            return bytesCount;
        }

        /**
         * Get frame data as uint8
         * @return
         */
        inline uint8_t *u8() const {
            return bytes;
        }

        /**
         * Get frame data as uint16
         * @return
         */
        inline uint16_t *u16() const {
            return bytes == NULL ? NULL : (uint16_t *) bytes;
        }

        /**
         * Test if frame is in grayscale mode
         * @return
         */
        inline bool isGray() const {
            return size() == width * height;
        }

        /**
         * Test if frame is in RGB565 mode
         * @return
         */
        inline bool isRGB() const {
            return size() == width * height * 2;
        }

        /**
         * Test if frame is in RGB888 mode
         * @return
         */
        inline bool isRaw() const {
            return size() == width * height * 3;
        }

        /**
         * Test if frame is in JPEG mode
         * @return
         */
        inline bool isJPEG() const {
            return !isGray() && !isRGB() && !isRaw();
        }

        /**
         * Set dimensions of frame
         * @param w
         * @param h
         */
        inline void setDimensions(const uint16_t w, const uint16_t h) {
            width = w;
            height = h;
        }

        /**
         * Update bytes buffer
         * @param b
         * @param count
         */
        inline void setBytes(uint8_t *b, const size_t count) {
            bytes = b;
            bytesCount = count;
        }

        /**
         * Swap uint16_t bytes order
         */
        void swapBytes() {
            espkit::functions::swapBytes(u16(), bytesCount / 2);
        }

    protected:
        size_t bytesCount;
        uint8_t *bytes;
    };
}