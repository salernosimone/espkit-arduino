#ifndef ELOQUENTESP32_INCLUDE_MOTION_JPEGHANDLER_H
#define ELOQUENTESP32_INCLUDE_MOTION_JPEGHANDLER_H

#include "../../../ignore/deprecated/Bytes.h"
#include "../../../ignore/deprecated/types.h"
#include "../JpegDecHandler.h"
#include "./Config.h"

namespace cam::motion {
    /**
     * Motion detection jpeg handler
     */
    class JpegHandler : public cam::JpegDecHandler {
    public:
        size_t total;
        size_t moving;
        Config *mconfig;
        Bytes background;

        /**
         * Constructor
         * @param c
         */
        JpegHandler(Config *c) :
                mconfig(c),
                total(0),
                moving(0),
                epoch(0) {

        }

        /**
         * Moving ratio
         * @return
         */
        inline float ratio() const {
            return total > 0 ? cast<float>(moving) / total : 0;
        }

        /**
         * Init decoding process
         * @param buf
         * @param len
         * @return
         */
        Status &open(uint8_t *buf, size_t len) {
            total = 0;
            moving = 0;

            return openT<JpegHandler>(buf, len);
        }

    protected:
        size_t epoch;

        /**
         * On open success, allocate memory for background
         */
        void onOpenSuccess() {
            grayscale();

            // choose scale based on input size
            if (w >= 1200) scale(16);
            else if (w >= 640) scale(8);
            else if (w >= 320) scale(4);
            else if (w >= 160) scale(2);
            else scale(1);

            background.resize(numOutputPixels(), w / config.scale);
        }

        /**
         *
         */
        void onDecodeSuccess() {
            epoch += 1;
        }

        /**
         * Process pixel
         * @param x
         * @param y
         * @param pixel
         * @return
         */
        bool onPixel(uint16_t x, uint16_t y, uint8_t pixel) {
            if (epoch == 0)
                background.set(y, x, pixel);
            else if (epoch < mconfig->config.train)
                background.smooth(y, x, pixel, mconfig->config.smooth);
            else if (mconfig->config.include(x, y, pixel)) {
                total += 1;
                moving += mconfig->config.compare(pixel, background.get(y, x)) ? 1 : 0;
                background.smooth(y, x, pixel, mconfig->config.smooth);
            }

            return true;
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_MOTION_JPEGHANDLER_H
