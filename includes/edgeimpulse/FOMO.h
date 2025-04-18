#pragma once

#include <edge-impulse-sdk/dsp/image/image.hpp>
#include "../support/Str.h"
#include "../camera/CameraFrame.h"
#include "./Impulse.h"
#include "./BBox.h"

using espkit::support::Str;
using espkit::video::Frame;

namespace espkit::edgeimpulse {
    /**
     * Object detection using FOMO
     */
    class FOMO : public Impulse {
    public:
        const uint16_t width = EI_CLASSIFIER_INPUT_WIDTH;
        const uint16_t height = EI_CLASSIFIER_INPUT_HEIGHT;
        const uint16_t channels = EI_CLASSIFIER_NN_INPUT_FRAME_SIZE / EI_CLASSIFIER_RAW_SAMPLE_COUNT;

        /**
         * Access results using [] notation
         * @param index
         * @return
         */
        BBox &operator[](uint8_t index) {
            const uint8_t maxCount = count();

            if (index >= maxCount) {
                bbox.empty();
            } else {
                uint8_t ix = 0;

                for (uint8_t i = 0; i < maxCount; i++) {
                    auto bb = results.bounding_boxes[i];

                    if (bb.value >= config.confidence.threshold)
                        ix += 1;

                    if (ix == index) {
                        bbox.label = bb.label;
                        bbox.confidence = bb.value;
                        bbox.x = bb.x;
                        bbox.y = bb.y;
                        bbox.width = bb.width;
                        bbox.height = bb.height;
                        bbox.cx = bb.x + bb.width / 2;
                        bbox.cy = bb.y + bb.height / 2;

                        break;
                    }
                }
            }

            return bbox;
        }

        /**
         * Get count of non background bounding boxes
         */
        uint8_t count() {
            uint8_t count = 0;

            for (uint8_t ix = 0; ix < results.bounding_boxes_count; ix++) {
                auto bb = results.bounding_boxes[ix];

                if (bb.value >= config.confidence.threshold)
                    count++;
            }

            return count;
        }

        /**
         * Run impulse on frame
         * Assume frame is either in grayscale or rgb565 mode
         * @param frame
         * @return
         */
        Status &run(Frame &frame) {
            if (frame.width < width || frame.height < height)
                return fail(
                        Str::from("Frame is too small for the Impulse. Expected size is ").concat(width, "x", height,
                                                                                                  " (got ", frame.width,
                                                                                                  "x", frame.height,
                                                                                                  ")"));

            startBenchmark();

            if (frame.isGray()) {
                // case 1: frame is gray, impulse accepts gray
                if (channels == 1)
                    return runGrayGray(&frame);

                // case 2: frame is gray, impulse accepts rgb
                return fail("Frame is grayscale, but Impulse requires RGB image");
            } else if (frame.isRGB()) {
                // case 3: frame is rgb, impulse accepts gray
                if (channels == 1)
                    return runColorGray(&frame);

                // case 4: frame is rgb, impulse accepts rgb
                return runColorColor(&frame);
            }

            return fail("Unknown frame format. Only grayscale and RGB565 are supported.");
        }

    protected:
        BBox bbox;

        /**
         * Prepare image
         * @param frame
         * @return
         */
        Status &runGrayGray(Frame *frame) {
//            cropGray(frame);

            return run();
        }

        /**
         * Prepare image
         * @param frame
         * @return
         */
        Status &runColorGray(Frame *frame) {
            cropColor(frame);

            return run();
        }

        /**
         * Prepare image
         * @param frame
         * @return
         */
        Status &runColorColor(Frame *frame) {
            cropColor(frame);

            uint16_t *bytes = frame->u16();

            signal.get_data = [bytes](size_t offset, size_t length, float *out) {
                for (size_t k = 0, i = offset, end = offset + length; i < end; k++, i++) {
                    const uint16_t pixel = bytes[i];
                    const uint8_t r5 = (pixel >> 11) & 0x1F;
                    const uint8_t g6 = (pixel >> 5) & 0x3F;
                    const uint8_t b5 = pixel & 0x1F;
                    const uint32_t r = (r5 << 3) | (r5 >> 2);
                    const uint16_t g = (g6 << 2) | (g6 >> 4);
                    const uint8_t b = (b5 << 3) | (b5 >> 2);

                    out[k] = (r << 16) | (g << 8) | b;
                }

                return EIDSP_OK;
            };

            return run();
        }

        /**
         * Actually run the impulse
         * @return
         */
        Status &run() {
            int status = run_classifier(&signal, &results, false);

            endBenchmark();

            if (status != EI_IMPULSE_OK)
                return failWithCode("run_classifier() failed", status);

            return succeed();
        }

        /**
         *
         * @param frame
         */
        void cropColor(Frame *frame) {
            if (frame->width == width && frame->height == height)
                return;

            // crop in-place, squashing
            const float dx = frame->width / width;
            const float dy = frame->height / height;
            uint16_t *bytes = frame->u16();
            size_t k = 0;

            for (float y = 0, i = 0; y < frame->height && i < height; y += dy, i += 1) {
                const size_t yOffset = ((size_t) y) * frame->width;

                for (float x = 0, j = 0; x < frame->width && j < width; x += dx, j += 1)
                    bytes[k++] = bytes[yOffset + ((int) x)];
            }

            frame->setBytes((uint8_t *) bytes, width * height * 2);
            frame->setDimensions(width, height);
        }
    };
}

// global singleton
static espkit::edgeimpulse::FOMO impulse;