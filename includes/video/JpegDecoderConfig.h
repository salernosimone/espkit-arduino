#pragma once

#if !defined(__JPEGDEC__)
#error "You must install and include the JPEGDEC library"
#endif

#include <JPEGDEC.h>

namespace espkit::video {
    /**
     * Configuration for JPEG decoders
     */
    class JpegDecoderConfig {
    public:
        struct {
            uint8_t scale;
            float stride;
            uint32_t flags;
            struct {
                uint16_t width;
                uint16_t height;
            } src, target;
        } values;

        /**
         * Set source dimensions (for scaling)
         * @param width
         * @param height
         */
        inline void fromDimensions(const uint16_t width, const uint16_t height) {
            values.src.width = width;
            values.src.height = height;
        }

        /**
         * Set target dimensions (for scaling)
         * @param width
         * @param height
         */
        inline void toDimensions(const uint16_t width, uint16_t height) {
            values.target.width = width;
            values.target.height = height;

            const uint8_t sx = constrain(((float) values.src.width) / width, 1, 999);
            const uint8_t sy = constrain(((float) values.src.height) / height, 1, 999);

            scale(sx < sy ? sx : sy);
        }

        /**
         * Set scale factor
         * @param s
         */
        void scale(const uint8_t s) {
            const uint8_t scale2 = (s >= 8 ? s : (s >= 4 ? 4 : (s >= 2 ? 2 : 1)));

            if (scale2 > 8) {
                ESP_LOGW("JpegDecHandler", "Max scale is 8. Will switch to scale + stride");
                stride(((float) scale2) / 8);
            }

            values.scale = constrain(scale2, 1, 8);
        }

        /**
         * Set stride
         * @param s
         */
        inline void stride(const float s) {
            values.stride = constrain(s, 1, 99999L);
        }

        /**
         * Set/Get custom flags
         */
        uint32_t flags(const uint32_t flags = 0) {
            values.flags |= flags;

            if (values.scale > 1)
                values.flags |= values.scale;

            return values.flags;
        }

        /**
         * Decode in grayscale
         */
        inline void grayscale() {
            flags(values.flags | JPEG_LUMA_ONLY);
        }

        /**
         * Check if decoding is in grayscale mode
         * @return
         */
        inline bool isGrayscale() const {
            return values.flags & JPEG_LUMA_ONLY;
        }
    };
}