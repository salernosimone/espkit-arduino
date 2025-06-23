#pragma once

#if !defined(__JPEGDEC__)
#error "You must install and include the JPEGDEC library"
#endif

#include <JPEGDEC.h>
#include "../support/HasOpStatus.h"
#include "./JpegDecoderConfig.h"

using espkit::support::Status;
using espkit::support::HasStatus;

namespace espkit::video {
    /**
     * Base class for custom JPEG decoders
     */
    class JpegDecoder : HasStatus {
    public:
        JPEGDEC decoder;
        JpegDecoderConfig config;

        /**
         * Do decoding
         */
         template<typename Impl>
        Status &decode(uint8_t *buf, size_t len) {
            if (!decoder.openFLASH(buf, len, [](JPEGDRAW *mcu) -> int {
                ESP_LOGI("jpeg", "mcu y=%d, x=%d, w=%d, h=%d", mcu->y, mcu->x, mcu->iWidthUsed, mcu->iHeight);
                Impl *impl = (Impl*) (mcu->pUser);

                return impl->onMCU(mcu);
            }))
                return decodeStatus(decoder.getLastError());

            // set crop area
            if (config.values.src.width > config.values.target.width ||
                config.values.src.height > config.values.target.height) {
                const uint8_t scale = constrain(config.values.scale, 1, 99);
                const uint16_t dx = config.values.src.width / scale - config.values.target.width;
                const uint16_t dy = config.values.src.height / scale - config.values.target.height;

                decoder.setCropArea(dx / 2, dy / 2, config.values.target.width * scale, config.values.target.height * scale);
            }

            decoder.getCropArea(&(cropArea.x), &(cropArea.y), &(cropArea.width), &(cropArea.height));
            cropArea.bpp = config.isGrayscale() ? 1 : 2;

            onOpenSuccess();
            decoder.setUserPointer((void *) this);
            decoder.decode(0, 0, config.flags());
            decoder.close();
            onDecodeDone();

            return decodeStatus(decoder.getLastError());
        }

        /**
         * Parse MCU
         * @param mcu
         * @return
         */
        virtual bool onMCU(JPEGDRAW *mcu) = 0;

    protected:
        struct {
            int x;
            int y;
            int width;
            int height;
            uint8_t bpp;
        } cropArea;

        /**
         * Run function on JPEG open success
         * (meant to be overriden)
         */
        virtual void onOpenSuccess() {

        }

        /**
         * Run function on JPEG decode done
         * (meant to be overriden)
         */
        virtual void onDecodeDone() {

        }

        /**
         * Decode JPEGDEC status code
         */
        Status &decodeStatus(int code) {
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