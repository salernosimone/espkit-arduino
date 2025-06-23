#ifndef ELOQUENTESP32_INCLUDE_FRAME_H
#define ELOQUENTESP32_INCLUDE_FRAME_H

#include <Arduino.h>
#include <esp_camera.h>
#include "../idf/xLock.h"
#include "../support/HasOpStatus.h"
#include "../video/Frame.h"

using espkit::idf::xLock;
using espkit::support::Status;
using espkit::support::HasStatus;
using espkit::video::Frame;

namespace espkit::cam {
    /**
     * A frame from the camera
     */
    class CameraFrame : public HasStatus, public Frame {
    public:
        xLock lock;

        camera_fb_t *fb;

        /**
         * Constructor
         */
        CameraFrame() : lock("frame"), fb(NULL) {

        }

        /**
         * Check if frame is empty
         * @return
         */
        operator bool() const {
            return size() > 0;
        }

        /**
         *
         * @param w
         * @param h
         * @return
         */
        Status grab(const uint16_t w, const uint16_t h) {
            // capture in isolation
            lock.acquire();
            free();
            fb = esp_camera_fb_get();
            lock.release();

            if (fb == NULL || fb->buf == NULL || fb->len == 0)
                return fail("Cannot capture frame");

            setBytes(fb->buf, fb->len);
            setDimensions(w, h);

            return succeed();
        }

        /**
         * Release frame resources
         */
        void free() {
            if (fb != NULL) {
                esp_camera_fb_return(fb);
                fb = NULL;
            }
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_FRAME_H
