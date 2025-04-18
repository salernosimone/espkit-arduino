#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../../ignore/deprecated/str.h"
#include "./Brownout.h"
#include "./Pinout.h"
#include "./Pixformat.h"
#include "./Quality.h"
#include "./Resolution.h"
#include "./XCLK.h"

namespace espkit::cam {
    /**
     * Camera config entrypoint
     */
    class Config {
    public:
        camera_config_t config;
        Brownout brownout;
        Pinout pinout;
        Pixformat pixformat;
        Quality quality;
        Resolution resolution;
        XCLK clock;

        /**
         * Constructor
         */
        Config() : pinout(&config) {

        }

        /**
         * Allow configuration from string
         * @param configString
         */
        void operator()(const String &configString) {
            str::split(configString, " ", [this](uint16_t i, String conf) {
                conf.trim();

                configureClock(conf) ||
                configurePixformat(conf) ||
                configureQuality(conf) ||
                configurePinout(conf) ||
                configureResolution(conf) ||
                warnUnknown(conf);
            });
        }

    protected:

        /**
         *
         * @param conf
         */
        bool configureClock(const String &conf) {
            if (!conf.startsWith("clock:"))
                return false;

            if (conf.endsWith("fast"))
                clock.fast();
            else if (conf.endsWith("slow"))
                clock.slow();

            return true;
        }

        /**
         *
         * @param conf
         */
        bool configurePixformat(const String &conf) {
            if (!conf.startsWith("format:"))
                return false;

            if (conf.endsWith("jpeg")) {
                pixformat.jpeg();
                return true;
            } else if (conf.endsWith("rgb")) {
                pixformat.rgb();
                return true;
            } else if (conf.endsWith("raw")) {
                pixformat.raw();
                return true;
            } else if (conf.endsWith("gray")) {
                pixformat.gray();
                return true;
            }

            return false;
        }

        /**
         *
         * @param conf
         */
        bool configureQuality(const String &conf) {
            if (!conf.startsWith("q:"))
                return false;

            quality.set(conf.substring(2));

            return true;
        }

        /**
         *
         * @param conf
         */
        bool configurePinout(const String &conf) {
            String name = conf.substring(7);

            if (conf == "pinout:guess") {
                pinout.guess();
                return true;
            } else if (conf == "pinout:prompt") {
                pinout.prompt();
                return true;
                // todo:
//            } else if (pinout.isValidName(name)) {
//                pinout.setFromName(name);
//                return true;
            }

            return false;
        }

        /**
         *
         * @param conf
         */
        bool configureResolution(const String &conf) {
            if (!conf.startsWith("res:"))
                return false;

            String res = conf.substring(4);

            if (resolution.isValidName(res)) {
                resolution.setFromName(res);
                return true;
            } else if (res == "prompt") {
                resolution.prompt();
                return true;
            }

            return false;
        }

        /**
         *
         * @param conf
         * @return
         */
        bool warnUnknown(const String &conf) {
            ESP_LOGE("Camera.Config", "Unknown conf value: %s", conf.c_str());

            return true;
        }
    };
}