#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../support/Str.h"
#include "../support/KV.h"
#include "./sensor/Setting.h"
#include "./sensor/BinarySetting.h"
#include "./sensor/RangeSetting.h"
#include "./sensor/BoundedSetting.h"
#include "./sensor/SpecialEffect.h"
#include "./sensor/FlipSetting.h"

using espkit::support::Str;
using espkit::support::KV;
using espkit::cam::sensor::Setting;
using espkit::cam::sensor::BinarySetting;
using espkit::cam::sensor::RangeSetting;
using espkit::cam::sensor::BoundedSetting;
using espkit::cam::sensor::SpecialEffect;
using espkit::cam::sensor::FlipSetting;

const uint8_t NUM_SETTINGS = 17;

namespace espkit::cam {
    /**
     *
     */
    class Sensor {
    public:
        sensor_t *sensor;
        BinarySetting gainControl;
        BinarySetting automaticWhiteBalance;
        BinarySetting exposureControl;
        BinarySetting automaticWhiteBalanceGain;
        BinarySetting automaticExposureControl;
        BinarySetting dcw;
        BinarySetting bpc;
        BinarySetting wpc;
        BinarySetting lensCorrection;
        BinarySetting rawGamma;
        RangeSetting brightness;
        RangeSetting saturation;
        BoundedSetting gain;
        BoundedSetting exposure;
        Setting gainCeiling;
        SpecialEffect specialEffect;
        FlipSetting flip;

        /**
         * Constructor
         */
        Sensor() :
                gainControl("gctrl", [](sensor_t *s, bool en) { s->set_gain_ctrl(s, en); }),
                automaticWhiteBalance("awb", [](sensor_t *s, bool en) { s->set_whitebal(s, en); }),
                automaticWhiteBalanceGain("awbg", [](sensor_t *s, bool en) { s->set_awb_gain(s, en); }),
                exposureControl("ectrl", [](sensor_t *s, bool en) { s->set_exposure_ctrl(s, en); }),
                automaticExposureControl("aec", [](sensor_t *s, bool en) { s->set_aec2(s, en); }),
                lensCorrection("lenc", [](sensor_t *s, bool en) { s->set_lenc(s, en); }),
                rawGamma("gamma", [](sensor_t *s, bool en) { s->set_raw_gma(s, en); }),
                dcw("dcw", [](sensor_t *s, bool en) { s->set_dcw(s, en); }),
                wpc("wpc", [](sensor_t *s, bool en) { s->set_wpc(s, en); }),
                bpc("bpc", [](sensor_t *s, bool en) { s->set_bpc(s, en); }),
                brightness("bright", [](sensor_t *s, float x) { s->set_brightness(s, x); }),
                saturation("sat", [](sensor_t *s, float x) { s->set_saturation(s, x); }),
                gain("gain", 30, [](sensor_t *s, float x) { s->set_agc_gain(s, x); }),
                exposure("exp", 1200, [](sensor_t *s, float x) { s->set_aec_value(s, x); }),
                gainCeiling("gceil", [](sensor_t *s, float x) { s->set_gainceiling(s, (gainceiling_t) x); }) {
        }

        /**
         * Init sensor instance
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
            Str::from(settings).explode(" ", [this](const String &s) {
                ESP_LOGI("tmp", "explode: %s, flip.name: %s", s.c_str(), flip.name.c_str());

                const KV kv(s);

                if (kv.key == specialEffect.name) specialEffect.set(kv.value);
                else if (kv.key == flip.name) flip.set(kv.value);
                else if (kv.key == brightness.name) brightness.set(kv.value);
                else if (kv.key == saturation.name) saturation.set(kv.value);
                else if (kv.key == automaticWhiteBalance.name) automaticWhiteBalance.set(kv.value);
                else if (kv.key == exposureControl.name) exposureControl.set(kv.value);
                else if (kv.key == automaticWhiteBalanceGain.name) automaticWhiteBalanceGain.set(kv.value);
                else if (kv.key == automaticExposureControl.name) automaticExposureControl.set(kv.value);
                else if (kv.key == dcw.name) dcw.set(kv.value);
                else if (kv.key == bpc.name) bpc.set(kv.value);
                else if (kv.key == wpc.name) wpc.set(kv.value);
                else if (kv.key == lensCorrection.name) lensCorrection.set(kv.value);
                else if (kv.key == rawGamma.name) rawGamma.set(kv.value);
                else if (kv.key == gain.name) gain.set(kv.value);
                else if (kv.key == exposure.name) exposure.set(kv.value);
                else if (kv.key == gainCeiling.name) gainCeiling.set(kv.value);
                else if (kv.key == gainControl.name) gainControl.set(kv.value);
                else ESP_LOGE("Camera.Sensor", "Unknown setting: %s", kv.key.c_str());
            });

//            str::split(settings, " ", [this](uint8_t i, const String &setting) {
//                if (setting.startsWith("effect:"))
//                    specialEffect.set(setting.substring(7));
//                else if (setting.startsWith("flip:"))
//                    flip(setting.substring(5));
//                else if (setting.startsWith("saturation:"))
//                    saturation.set(setting.substring(11));
//                else if (setting.startsWith("brightness:"))
//                    brightness.set(setting.substring(11));
//                else if (setting.startsWith("gainControl"))
//                    gainControl.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("awb"))
//                    automaticWhiteBalance.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("exposureControl"))
//                    exposureControl.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("awbGain"))
//                    automaticWhiteBalanceGain.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("autoExposureControl"))
//                    automaticExposureControl.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("dcw"))
//                    dcw.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("wpc"))
//                    wpc.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("lensCorretion"))
//                    lensCorrection.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("rawGamma"))
//                    rawGamma.set(!setting.endsWith(":no"));
//                else if (setting.startsWith("exposure:"))
//                    exposure.set(str::after(setting, ":").toFloat());
//                else if (setting.startsWith("gain:"))
//                    gain.set(str::after(setting, ":").toFloat());
//                else if (setting.startsWith("gainCeling:"))
//                    gainCeiling.set(str::after(setting, ":").toFloat());
//                else ESP_LOGE("Camera.Sensor", "Unknown setting: %s", setting.c_str());
//            });
        }

        /**
         * Called from Camera::begin()
         * @param framesize
         */
        void frameSize(framesize_t framesize) {
            begin();

            sensor->set_framesize(sensor, framesize);
        }
    };
}