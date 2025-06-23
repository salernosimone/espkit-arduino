#pragma once

#include "../support/Arena.h"
#include "./JpegDecoder.h"

using espkit::support::Arena;

namespace espkit::video {
    /**
     * Keep in memory decoded image
     */
    class MemoryJpegDecoder : public JpegDecoder {
    public:
        Arena arena;

        /**
         * Parse mcu
         * @param mcu
         * @return
         */
        bool onMCU(JPEGDRAW *mcu) {
            const size_t widthUsed = std::min(cropArea.width, mcu->iWidthUsed);
            const uint8_t bpp = cropArea.bpp;
            const size_t offset = cropArea.width * mcu->y * bpp;
            const uint8_t *pixels = (uint8_t *) mcu->pPixels;

            for (size_t y = 0; y < mcu->iHeight; y++) {
                const size_t dy = y * mcu->iWidthUsed * bpp;
                memcpy(arena.data + offset + y * widthUsed * bpp, pixels + dy, widthUsed * bpp);
            }

            return true;
        }

    protected:

        /**
         * Allocate memory
         */
        void onOpenSuccess() {
            arena.alloc(cropArea.width * cropArea.height * cropArea.bpp);
            arena.zero();
        }
    };
}