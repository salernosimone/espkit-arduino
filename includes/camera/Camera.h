#pragma once

#include "../support/traits/HasStatus.h"
#include "./config/Config.h"
#include "./Sensor.h"
#include "./CameraFrame.h"

using espkit::support::HasStatus;
using espkit::cam::CameraFrame;

/**
 * Camera interface
 */
namespace espkit::cam {
    class Camera : public HasStatus {
    public:
        Config config;
        Sensor sensor;
        CameraFrame frame;

        /**
         * Init camera
         * @return
         */
        Status &begin() {
            if (!config.pinout)
                return status.fail("No pinout set");

            config.config.ledc_channel = LEDC_CHANNEL_0;
            config.config.ledc_timer = LEDC_TIMER_0;
            config.config.fb_count = 1;
            config.config.pixel_format = config.pixformat.format;
            config.config.frame_size = config.resolution.framesize;
            config.config.jpeg_quality = config.quality.quality;
            config.config.xclk_freq_hz = config.clock.freq;
            config.config.grab_mode = CAMERA_GRAB_LATEST;

            // deinit first
            esp_camera_deinit();

            if (esp_camera_init(&(config.config)) != ESP_OK)
                return status.fail("esp_camera_init() failed");

            // init sensor
            sensor.begin();

            if (config.resolution.framesize != FRAMESIZE_INVALID)
                sensor.frameSize(config.resolution.framesize);

            // init queue
            frameQueue = xQueueCreate(10, sizeof(Frame *));

            return status.succeed();
        }

        /**
         * Grab new frame
         * @return
         */
        CameraFrame &grab() {
            frame.grab(config.resolution.width, config.resolution.height);

            return frame;
        }

        /**
         * Add current frame to queue
         * @return
         */
        Status &queue() {
            if (frameQueue == 0)
                return fail("Frame queue not available");

            if (xQueueSendToFront(frameQueue, (void *) (&frame), (TickType_t) 100) != pdPASS)
                return fail("Can't queue frame");

            return succeed();
        }

    protected:
        QueueHandle_t frameQueue;
    };
}

// global singleton
static espkit::cam::Camera camera;