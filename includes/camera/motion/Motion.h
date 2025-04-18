#ifndef ELOQUENTESP32_INCLUDES_MOTION_H
#define ELOQUENTESP32_INCLUDES_MOTION_H

#include <functional>
#include <JPEGDEC.h>
#include "../../../ignore/deprecated/Benchmark.h"
#include "../../../ignore/deprecated/Throttle.h"
#include "../../support/traits/HasStatus.h"
#include "./Config.h"
#include "./JpegHandler.h"

namespace cam {
    /**
     * Motion detector
     */
    class Motion : public HasStatus {
    public:
        motion::Config config;
        motion::JpegHandler jpeg;
        Benchmark benchmark;
        Throttle throttle;

        /**
         * Constructor
         */
        Motion() : jpeg(&config), cb(NULL), cbWithArg(NULL) {

        }

        /**
         * Convert to bool
         * @return
         */
        explicit operator bool() {
            if (!happened())
                return false;

            throttle.touch();
            return true;
        }

        /**
         * Get moving point ratio
         * @return
         */
        inline float ratio() const {
            return jpeg.ratio();
        }

        /**
         * Return count of included pixels
         * @return
         */
        inline size_t count() const {
            return jpeg.total;
        }

        /**
         * Return count of moving pixels
         * @return
         */
        inline size_t moving() const {
            return jpeg.moving;
        }

        /**
         * Check if motion happened
         * @return
         */
        inline bool happened() const {
            return !failed() && throttle.allowed() && ratio() >= config.config.threshold;
        }

        /**
         * Detect on frame
         * @tparam Frame
         * @param frame
         * @return
         */
        template<typename Frame>
        Status &detect(Frame &frame) {
            return detect(frame.u8(), frame.size());
        }

        /**
         * Detect on jpeg raw data
         * @param jpegdata
         * @param length
         * @return
         */
        Status &detect(uint8_t *jpegdata, size_t length) {
            benchmark.start();

            if (!jpeg.open(jpegdata, length))
                return benchmark.stop<Status &>(jpeg.status);

            if (!jpeg.decode())
                return benchmark.stop<Status &>(jpeg.status);

            benchmark.stop();
            throttle.increment();

            if (happened()) {
                if (cb) cb();
                if (cbWithArg) cbWithArg(*this);
            }

            return succeed();
        }

        /**
         * Add event listener to be called when motion happens
         * @param callback
         */
        void addEventListener(std::function<void(void)> callback) {
            cb = callback;
        }

        /**
         * Add event listener to be called when motion happens
         * @param callback
         */
        void addEventListener(std::function<void(Motion &)> callback) {
            cbWithArg = callback;
        }

    protected:
        std::function<void(void)> cb;
        std::function<void(Motion &)> cbWithArg;
    };
}

#endif //ELOQUENTESP32_INCLUDES_MOTION_H
