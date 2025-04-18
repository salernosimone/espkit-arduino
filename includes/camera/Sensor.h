#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../ignore/deprecated/str.h"
#include "../../ignore/deprecated/validation.h"
#include "./sensor/Setting.h"
#include "./sensor/SpecialEffect.h"

using cam::sensor::Setting;
using cam::sensor::SpecialEffect;

#define __INIT_BINARY_SETTING__(settingName, setter) settingName(sensor, [](sensor_t *s, float enabled) { s->setter(s, enabled > 0); })

namespace espkit::cam {
    /**
     *
     */
    class Sensor {
    public:
        sensor_t *sensor;
        Setting gainControl;
        Setting automaticWhiteBalance;
        Setting exposureControl;
        Setting automaticWhiteBalanceGain;
        Setting automaticExposureControl;
        Setting dcw;
        Setting bpc;
        Setting wpc;
        Setting lensCorrection;
        Setting rawGamma;
        Setting gain;
        Setting exposure;
        Setting gainCeiling;
        Setting brightness;
        Setting saturation;
        SpecialEffect specialEffect;

        /**
         * Constructor
         */
        Sensor() :
                specialEffect(sensor),
                __INIT_BINARY_SETTING__(gainControl, set_gain_ctrl),
                __INIT_BINARY_SETTING__(automaticWhiteBalance, set_whitebal),
                __INIT_BINARY_SETTING__(automaticWhiteBalanceGain, set_awb_gain),
                __INIT_BINARY_SETTING__(exposureControl, set_exposure_ctrl),
                __INIT_BINARY_SETTING__(automaticExposureControl, set_aec2),
                __INIT_BINARY_SETTING__(lensCorrection, set_lenc),
                __INIT_BINARY_SETTING__(rawGamma, set_raw_gma),
                __INIT_BINARY_SETTING__(dcw, set_dcw),
                __INIT_BINARY_SETTING__(wpc, set_wpc),
                __INIT_BINARY_SETTING__(bpc, set_bpc),
                brightness(sensor, [](sensor_t *s, float value) {
                    s->set_brightness(s, value);
                }),
                saturation(sensor, [](sensor_t *s, float value) {
                    s->set_saturation(s, value);
                }),
                gain(sensor, [](sensor_t *s, float value) {
                    // assume value is in the range 0-100
                    // original range is 0 - 30
                    if (validate("gain must be in the range [0, 100] (inclusive)", value <= 100))
                        s->set_agc_gain(s, value / 100.0f * 30);
                }),
                exposure(sensor, [](sensor_t *s, float value) {
                    // assume value is in the range 0-100
                    // original range is 0 - 1200
                    if (validate("exposure must be in the range [0, 100] (inclusive)", value <= 100))
                        s->set_aec_value(s, value / 100.0f * 1200);
                }),
                gainCeiling(sensor, [](sensor_t *s, float value) {
                    // original range is 0 - 6
                    if (validate("gain ceiling must be in the range [0, 6] (inclusive)", value <= 6))
                        s->set_gainceiling(s, (gainceiling_t) value);
                }) {

        }

        /**
         * Get sensor instance
         */
        void begin() {
            if (sensor == NULL)
                sensor = esp_camera_sensor_get();
        }

        /**
         * Set from string
         * @param setting
         */
        void set(const String &settings) {
            str::split(settings, " ", [this](uint8_t i, const String &setting) {
                if (setting.startsWith("effect:"))
                    specialEffect.set(setting.substring(7));
                else if (setting.startsWith("flip:"))
                    flip(setting.substring(5));
                else if (setting.startsWith("saturation:"))
                    saturation.set(setting.substring(11));
                else if (setting.startsWith("brightness:"))
                    brightness.set(setting.substring(11));
                else if (setting.startsWith("gainControl"))
                    gainControl.set(!setting.endsWith(":no"));
                else if (setting.startsWith("awb"))
                    automaticWhiteBalance.set(!setting.endsWith(":no"));
                else if (setting.startsWith("exposureControl"))
                    exposureControl.set(!setting.endsWith(":no"));
                else if (setting.startsWith("awbGain"))
                    automaticWhiteBalanceGain.set(!setting.endsWith(":no"));
                else if (setting.startsWith("autoExposureControl"))
                    automaticExposureControl.set(!setting.endsWith(":no"));
                else if (setting.startsWith("dcw"))
                    dcw.set(!setting.endsWith(":no"));
                else if (setting.startsWith("wpc"))
                    wpc.set(!setting.endsWith(":no"));
                else if (setting.startsWith("lensCorretion"))
                    lensCorrection.set(!setting.endsWith(":no"));
                else if (setting.startsWith("rawGamma"))
                    rawGamma.set(!setting.endsWith(":no"));
                else if (setting.startsWith("exposure:"))
                    exposure.set(str::after(setting, ":").toFloat());
                else if (setting.startsWith("gain:"))
                    gain.set(str::after(setting, ":").toFloat());
                else if (setting.startsWith("gainCeling:"))
                    gainCeiling.set(str::after(setting, ":").toFloat());
                else ESP_LOGE("Camera.Sensor", "Unknown setting: %s", setting.c_str());
            });
        }

        /**
         *
         * @param direction
         */
        void flip(const String &direction) {
            begin();

            if (direction.startsWith("vert")) {
                sensor->set_vflip(sensor, true);
            } else if (direction.startsWith("hor")) {
                sensor->set_hmirror(sensor, true);
            } else if (direction == "both") {
                flip("vertical");
                flip("horizontal");
            } else {
                sensor->set_vflip(sensor, false);
                sensor->set_hmirror(sensor, false);
            }
        }

        /**
         *
         * @param framesize
         */
        void frameSize(framesize_t framesize) {
            begin();

            sensor->set_framesize(sensor, framesize);
        }
    };
}