#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../idf/SerialKit.h"
#include "./Resolution/SupportedResolutions.h"

using espkit::cam::resolution::Definition;
using espkit::cam::resolution::supportedResolutions;
using espkit::cam::resolution::supportedResolutionsCount;

namespace espkit::cam::config {
    /**
     * Frame resolution
     */
    class Resolution {
    public:
        framesize_t framesize;
        uint16_t width;
        uint16_t height;

        /**
         * Constructor
         */
        Resolution() : name("") {
            vga();
        }

        /**
         * Check if resolution is custom
         * @return
         */
        inline bool isCustom() const {
            return framesize == FRAMESIZE_INVALID;
        }

        /**
         * Test if given name is a valid resolution
         * @param name
         * @return
         */
        bool isValidName(String name) {
            for (uint8_t i = 0; i < supportedResolutionsCount; i++)
                if (supportedResolutions[i]->matches(name))
                    return true;

            return false;
        }

        /**
         * Set resolution from name
         * @param name
         */
        bool set(const String &name) {
            if (name == "prompt") {
                prompt();
                return true;
            }

            for (uint8_t i = 0; i < supportedResolutionsCount; i++) {
                if (supportedResolutions[i]->matches(name)) {
                    const Definition *resolution = supportedResolutions[i];

                    set(resolution->size, resolution->width, resolution->height);
                    this->name = resolution->name;

                    return true;
                }
            }

            ESP_LOGE("Camera.Config.Resolution", "Unknown resolution: %s", name.c_str());

            return false;
        }

        /**
         * Ask user to select a resolution from a list
         */
        void prompt() {
            static String choices[supportedResolutionsCount];

            for (uint8_t i = 0; i < supportedResolutionsCount; i++)
                choices[i] = supportedResolutions[i]->dimensions();

            const String resolution = serialkit.promptChoice("Select frame resolution:", (String *) choices,
                                                             supportedResolutionsCount);

            set(resolution);
            ESP_LOGI("Camera.Config.Resolution",
                     "You can skip this step in your sketch by adding camera.config.resolution.%s();", name.c_str());
        }

        /**
         * Set size
         * @param framesize
         * @param width
         * @param height
         */
        void set(framesize_t framesize, uint16_t width, uint16_t height) {
            this->framesize = framesize;
            this->width = width;
            this->height = height;
        }

        /**
         * Set custom resolution
         * @param width
         * @param height
         */
        void custom(uint16_t width, uint8_t height) {
            set(FRAMESIZE_INVALID, width, height);
        }

        inline void yolo() { set("yolo"); }

        inline void qqvga() { set("qqvga"); }

        inline void yoloxl() { set("yoloxl"); }

        inline void qcif() { set("qcif"); }

        inline void hqvga() { set("hqvga"); }

        inline void square() { set("square"); }

        inline void qvga() { set("qvga"); }

        inline void squarexl() { set("squarexl"); }

        inline void cif() { set("cif"); }

        inline void hvga() { set("hvga"); }

        inline void vga() { set("vga"); }

        inline void svga() { set("svga"); }

        inline void xga() { set("xga"); }

        inline void hd() { set("hd"); }

        inline void sxga() { set("sxga"); }

        inline void uxga() { set("uxga"); }

        inline void fhd() { set("fhd"); }

        inline void p_hd() { set("p_hd"); }

        inline void p_3mp() { set("p_3mp"); }

        inline void qxga() { set("qxga"); }

        inline void qhd() { set("qhd"); }

        inline void wqxga() { set("wqxga"); }

        inline void p_fhd() { set("p_fhd"); }

        inline void qsxga() { set("qsxga"); }

        inline void pi() { set("pi"); }

    protected:
        String name;
    };
}