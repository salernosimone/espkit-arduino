#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../support/Str.h"
#include "../../support/KV.h"
#include "./Brownout.h"
#include "./Pinout.h"
#include "./Pixformat.h"
#include "./Quality.h"
#include "./Resolution.h"
#include "./XCLK.h"
#include "./ByteOrder.h"

using espkit::support::Str;
using espkit::support::KV;

namespace espkit::cam::config {
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
        ByteOrder byteOrder;

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
            Str::from(configString).explode(" ", [this](const String &conf) {
                const KV kv(conf);

                Serial.print("conf=");
                Serial.println(conf);

                if (kv == "pin") pinout.set(kv.value);
                else if (kv == "pix") pixformat.set(kv.value);
                else if (kv == "q") quality.set(kv.value);
                else if (kv == "res") resolution.set(kv.value);
                else if (kv == "xclk") clock.set(kv.value);
                else ESP_LOGE("Camera.Config", "Unknown configuration: %s", String(conf).c_str());
            });

//            str::split(configString, " ", [this](uint16_t i, String conf) {
//                conf.trim();
//
//                configureClock(conf) ||
//                configurePixformat(conf) ||
//                configureQuality(conf) ||
//                configurePinout(conf) ||
//                configureResolution(conf) ||
//                warnUnknown(conf);
//            });
        }

    protected:
    };
}