#pragma once

#include <human_face_detect_msr01.hpp>
#include <human_face_detect_mnp01.hpp>
#include "../../constants.h"
#include "../../functions/swapBytes.h"
#include "../../support/OpStatus.h"
#include "../../support/HasOpStatus.h"
#include "../../support/traits/HasLatency.h"
#include "../../video/Frame.h"
#include "../Camera.h"
#include "./FaceBBox.h"
#include "./config/FaceDetectionConfig.h"

#if CAN_DECODE_JPEG
#include "../../video/MemoryJpegDecoder.h"
using espkit::video::MemoryJpegDecoder;
#endif

using espkit::support::Status;
using espkit::support::HasStatus;
using espkit::support::HasLatency;
using espkit::video::Frame;
using espkit::cam::face::config::FaceDetectionConfig;

namespace espkit::cam::face {
    /**
     * Detect (not recognize!) faces
     */
    class FaceDetection : public HasStatus, public HasLatency {
    public:
        FaceDetectionConfig config;

        /**
         * Access results using [] notation
         * @param index
         * @return
         */
        FaceBBox &operator[](uint8_t index) {
            static FaceBBox empty;

            if (index >= _count)
                return empty;

            return bboxes[index];
        }

        /**
         * Get count of valid faces
         */
        inline uint8_t count() const {
            return _count;
        }

        /**Frame
         *
         * @param frame
         * @return
         */
        Status &detect(Frame &frame) {
            if (frame.isRGB())
                return detectRGB(frame);

            if (frame.isJPEG()) {
#if !CAN_DECODE_JPEG
                return fail("To detect faces in JPEG images you must include the JPEGDEC library.");
#else
                return detectJPEG(frame);
#endif
            }

            return fail("Unknown frame format. JPEG and RGB (565 or 888) are supported");
        }

    protected:
        uint8_t _count;
        FaceBBox bboxes[MAX_FACES];
#if CAN_DECODE_JPEG
        MemoryJpegDecoder jpegDecoder;

        /**
         *
         * @param frame
         * @return
         */
        Status &detectJPEG(Frame &frame) {
            if (frame.width < 240 || frame.height < 240)
                return fail("Frame must be at least 240x240");

            const float scalex = frame.width / 240;
            const float scaley = frame.height / 240;
            const float scale = scalex < scaley ? scalex : scaley;

            jpegDecoder.config.fromDimensions(frame.width, frame.height);
            jpegDecoder.config.toDimensions(240, 240);
            jpegDecoder.decode<MemoryJpegDecoder>(frame.u8(), frame.size());

            // swap bytes
            uint16_t *image = (uint16_t*) jpegDecoder.arena.data;

            Serial.println("Frame: ");
            for (size_t i = 0; i < 240 * 240; i++) {
                Serial.print(image[i]);
                Serial.print(',');
            }
            Serial.println();
            espkit::functions::swapBytes(image, 240 * 240 * 2);

            return detect(image, 240, 240);
        }
#endif

        /**
         *
         * @param frame
         */
        Status &detectRGB(Frame &frame) {
            if (frame.width != 240 || frame.height != 240)
                return fail("Only 240x240 RGB image is supported");

            return detect(frame.u16(), frame.width, frame.height);
        }

        /**
         * Actual detection
         * @param image
         * @param width
         * @param height
         * @return
         */
        Status &detect(uint16_t *image, uint16_t width, uint16_t height) {
            // we assume that image is in "idf" ordering (swapped bytes!)
            // since swapping/unswapping would require 15ms
            startBenchmark();
            std::vector<int> shape = {(int) width, (int) height, 3};
            std::list<dl::detect::result_t> results;
            HumanFaceDetectMSR01 msr(
                    config.msr.score,
                    config.msr.nms,
                    config.msr.topK,
                    config.msr.resizeScale
            );

            if (config.values.keypoints) {
                HumanFaceDetectMNP01 mnp(
                        config.mnp.score,
                        config.mnp.nms,
                        config.mnp.topK
                );

                std::list<dl::detect::result_t> &candidates = msr.infer(image, shape);
                results = mnp.infer(image, shape, candidates);
            } else {
                results = msr.infer(image, shape);
            }

            selectValidFaces(&results);
            stopBenchmark();

            return succeed();
        }

        /**
         *
         * @param results
         */
        void selectValidFaces(std::list<dl::detect::result_t> *results) {
            _count = 0;

            for (std::list<dl::detect::result_t>::iterator bbox = results->begin();
                 bbox != results->end(); bbox++) {
                if (bbox->score >= config.confidence.threshold) {
                    bboxes[_count].confidence = bbox->score;
                    bboxes[_count].setCoordinates(bbox->box);

                    if (config.values.keypoints)
                        bboxes[_count].landmarks.set(bbox->keypoint);
                    else
                        bboxes[_count].landmarks.empty();

                    _count += 1;
                }
            }
        }
    };
}

static espkit::cam::face::FaceDetection facekit;