#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../PreferencesKit/PreferencesKit.h"
#include "../../idf/SerialKit.h"
#include "./Pinout/SupportedPinouts.h"
#include "../../../ignore/deprecated/str.h"

using espkit::cam::pinout::supportedPinouts;
using espkit::cam::pinout::supportedPinoutsCount;

#define PREF_KEY_PINOUT_GUESS "pinout_guess"
#define PREF_KEY_PINOUT_FOUND "pinout_found"

namespace espkit::cam::config {
    /**
     * Set camera pins
     */
    class Pinout {
    public:
        camera_config_t *config;

        /**
         * Constructor
         * @param config_
         */
        Pinout(camera_config_t *config_) : config(config_) {

        }

        /**
         * Syntactic sugar for if (!pinout)
         * @return
         */
        inline explicit operator bool() const {
            return isSet();
        }

        /**
         * Check if pins are set
         * @return
         */
        inline bool isSet() const {
            return config->pin_d0 >= 0 && config->pin_d1 >= 0 && config->pin_d0 != config->pin_d1;
        }

        /**
         * Forget guessed pinout
         */
        inline void forget() const {
            prefs.put(PREF_KEY_PINOUT_FOUND, "");
        }

        /**
         * Ask user to select a model from a list
         */
        void prompt() {
            static String choices[supportedPinoutsCount];

            for (uint8_t i = 0; i < supportedPinoutsCount; i++)
                choices[i] = supportedPinouts[i]->name;

            String name = serialkit.promptChoice("Select camera model:", (String *) choices, supportedPinoutsCount);

            set(name);
            ESP_LOGI("Camera.Config.Pinout",
                     "You can skip this step in your sketch by adding camera.config.pinout.%s();", name.c_str());

        }

        /**
         * Set pinout from model name
         * @param name
         */
        bool set(const String &name) {
            if (name == "prompt") {
                prompt();
                return true;
            }

            if (name == "guess") {
                guess();
                return true;
            }

            for (uint8_t i = 0; i < supportedPinoutsCount; i++) {
                if (supportedPinouts[i]->matches(name)) {
                    supportedPinouts[i]->applyTo(config);

                    return true;
                }
            }

            ESP_LOGE("Camera.Config.Pinout", "Unknown pinout: %s", name.c_str());

            return false;
        }

        /**
         * Guess pinout
         * WARNING: may cause reboots!
         */
        bool guess() {
            if (isSet())
                return true;

            String guessed = guessedName();

            // restore guessed pinout, if present
            if (guessed != "") {
                ESP_LOGI("Camera.Config.Pinout", "Restoring guessed pinout: %s", guessed.c_str());

                return set(guessed);
            }

            String next = nextInSequence();
            ESP_LOGI("Camera.Config.Pinout", "Guessing pinout: %s", next.c_str());
            prefs.put(PREF_KEY_PINOUT_GUESS, next);
            set(next);

            if (!init()) {
                ESP_LOGW("Pinout", "Can't init with pinout %s", next.c_str());
                return guess();
            }

            prefs.put(PREF_KEY_PINOUT_FOUND, next);
            ESP_LOGI("Camera.Config.Pinout",
                     "Was able to init pinout. You can skip this step in your sketch by adding camera.config.pinout.%s();",
                     next.c_str());

            return true;
        }

        inline void aithinker() { set("aithinker"); }

        inline void xiao() { set("xiao"); }

        inline void wroom_s3() { set("wroom_s3"); }

        inline void ttgo_plus() { set("ttgo_plus"); }

        inline void ttgo_pir() { set("ttgo_pir"); }

        inline void m5_default() { set("m5_default"); }

        inline void m5_fisheye() { set("m5_fisheye"); }

        inline void m5_timerx() { set("m5_timerx"); }

        inline void espeye() { set("espeye"); }

        inline void espeye_s3() { set("espeye_s3"); }

        inline void wrover() { set("wrover"); }

    protected:

        /**
         * Get name of guessed pinout
         * @return
         */
        inline String guessedName() {
            return prefs.getString(PREF_KEY_PINOUT_FOUND);
        }

        /**
         * Get next pinout to try
         * @param guessing
         * @return
         */
        String nextInSequence() {
            String guessing = prefs.getString(PREF_KEY_PINOUT_GUESS);

            for (uint8_t i = 0; i < supportedPinoutsCount; i++) {
                if (supportedPinouts[i]->matches(guessing)) {
                    return supportedPinouts[(i + 1) % supportedPinoutsCount]->name;
                }
            }

            return supportedPinouts[0]->name;
        }

        /**
         * Try to init with default configs
         * @return
         */
        bool init() {
            config->ledc_channel = LEDC_CHANNEL_0;
            config->ledc_timer = LEDC_TIMER_0;
            config->fb_count = 1;
            config->pixel_format = PIXFORMAT_JPEG;
            config->frame_size = FRAMESIZE_QQVGA;
            config->jpeg_quality = 20;
            config->xclk_freq_hz = 20000000ULL;
            config->grab_mode = CAMERA_GRAB_LATEST;

            // deinit first
            esp_camera_deinit();

            // may cause reboot!
            return esp_camera_init(config) == ESP_OK;
        }

        /**
         * Try to grab frame to be really sure pinout is correct
         * @return
         */
        bool getFrame() {
            delay(500);

            camera_fb_t *fb = esp_camera_fb_get();
            bool isOk = (fb != NULL && fb->buf != NULL && fb->len > 0);

            // release fb
            if (fb != NULL) {
                esp_camera_fb_return(fb);
                fb = NULL;
            }

            return isOk;
        }
    };
}