#if defined(__JPEGDEC__)
#ifndef ELOQUENTESP32_INCLUDE_JPEGDECHANDLER_H
#define ELOQUENTESP32_INCLUDE_JPEGDECHANDLER_H

#include <Arduino.h>
#include "../support/types.h"
#include "../support/traits/HasStatus.h"

namespace cam {
    /**
     * Custom JPEG decode logic
     */
    class JpegDecHandler : public HasStatus {
    public:
        JPEGDEC decoder;
        struct {
            uint8_t scale;
            float stride;
            uint32_t options;
        } config;

        /**
         * Set decode scale
         */
        void scale(uint8_t scale) {
            uint8_t s = pow(2, log2(scale > 1 ? scale : 1));

            if (s > 8) {
                ESP_LOGW("JpegDecHandler", "Max scale is 8. Will switch to scale + stride");
                s = 8;
            }

            config.scale = s;
            stride(cast<float>(scale) / s);
        }

        /**
         * Set decode stride
         */
        void stride(float stride) {
            if (stride < 1) {
                ESP_LOGW("JpegDecHandler", "Bad stride: %2.f. Must be >= 1", stride);
                return;
            }

            config.stride = stride;
        }

        /**
         * Set custom options
         */
        inline void options(uint32_t options) {
            config.options = options;
        }

        /**
         * Decode in grayscale
         */
        inline void grayscale() {
            options(config.options | JPEG_LUMA_ONLY);
        }

        /**
         * Get true image width
         */
        inline uint16_t width() {
            return w;
        }

        /**
         * Get true image height
         */
        inline uint16_t height() {
            return h;
        }

        /**
         * Get number of source image
         */
        inline size_t numInputPixels() const {
            return w * h;
        }

        /**
         * Get number of decoded pixels
         */
        inline size_t numOutputPixels() const {
            const float f = config.stride * config.scale;

            return ceil(numInputPixels() / f / f);
        }

        /**
         * Get decode flags
         */
        uint32_t flags() const {
          uint32_t flags = config.options;

          if (config.scale > 1)
            flags |= config.scale;

          return flags;
        }

        /**
         * Do decoding
         */
        Status& decode() {
            decoder.setUserPointer((void*) this);
            bool ok = decoder.decode(0, 0, flags());

            decoder.close();

            if (!ok) {
                decodeStatus(decoder.getLastError());
                onDecodeError();
            }
            else {
                status.succeed();
                onDecodeSuccess();
            }

            return status;
        }

    protected:
        size_t w;
        size_t h;

        /**
         * Start decoding process with custom child class
         */
        template<typename Impl>
        Status &openT(uint8_t *buf, size_t len) {
            w = 0;
            h = 0;

            bool ok = decoder.openFLASH(buf, len, [](JPEGDRAW *mcu) -> int {
                Impl *impl = (Impl*) mcu->pUser;

                return impl->onMCU(mcu);
            });

            if (!ok) {
                decodeStatus(decoder.getLastError());
                onOpenError();
            }
            else {
                w = decoder.getWidth();
                h = decoder.getHeight();

                onOpenSuccess();
            }

            return status;
        }

        /**
         * Called if open succeeded
         */
        virtual void onOpenSuccess() {

        }

        /**
         * Called if open failed
         */
        virtual void onOpenError() {

        }

        /**
         * Called if decode succeeded
         */
        virtual void onDecodeSuccess() {

        }

        /**
         * Called if decode failed
         */
        virtual void onDecodeError() {

        }

        /**
         * Run on each MCU
         * @return true if should continue decoding
         */
        virtual bool onMCU(JPEGDRAW *mcu) {
            // todo: handle stride
            for (float i = 0, y = mcu->y; i < mcu->iHeight; i += config.stride, y += config.stride) {
                if (!onRow(mcu, y))
                    return 0;

                const size_t offset = ((size_t) i) * mcu->iWidthUsed;
                const uint8_t *row = ((uint8_t*) mcu->pPixels) + offset;

                for (float j = 0, x = mcu->x; j < mcu->iWidthUsed; j += config.stride, x += config.stride) {
                    if (!onPixel(x / config.stride, y / config.stride, row[cast<size_t>(j)]))
                        return 0;
                }
          }

          return 1;
        }

        /**
         * Run on each row
         * @return true if should continue decoding
         */
        virtual bool onRow(JPEGDRAW *mcu, uint16_t y) {
          return true;
        }

        /**
         * Run on each pixel
         * @return true if should continue decoding
         */
        virtual bool onPixel(uint16_t x, uint16_t y, uint8_t pixel) {
            return true;
        }

        /**
         * Decode JPEGDEC status code
         */
        Status& decodeStatus(int code) {
            if (code == 0)
                return status.succeed();

            switch (code) {
                case JPEG_INVALID_PARAMETER:
                    return status.fail("Invalid parameter");
                case JPEG_DECODE_ERROR:
                    return status.fail("Decode error");
                case JPEG_UNSUPPORTED_FEATURE:
                    return status.fail("Unsupported feature");
                case JPEG_INVALID_FILE:
                    return status.fail("Invalid file");
            }

            return status.fail("Unknown error");
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_JPEGDECHANDLER_H
#endif