#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "../../idf/SerialKit.h"
#include "./Resolution/SupportedResolutions.h"

using espkit::cam::resolution::Definition;
using espkit::cam::resolution::supportedResolutions;
using espkit::cam::resolution::supportedResolutionsCount;

namespace espkit::cam {
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
        bool setFromName(const String &name) {
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

            setFromName(resolution);
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

        inline void yolo() { setFromName("yolo"); }

        inline void qqvga() { setFromName("qqvga"); }

        inline void yoloxl() { setFromName("yoloxl"); }

        inline void qcif() { setFromName("qcif"); }

        inline void hqvga() { setFromName("hqvga"); }

        inline void square() { setFromName("square"); }

        inline void qvga() { setFromName("qvga"); }

        inline void squarexl() { setFromName("squarexl"); }

        inline void cif() { setFromName("cif"); }

        inline void hvga() { setFromName("hvga"); }

        inline void vga() { setFromName("vga"); }

        inline void svga() { setFromName("svga"); }

        inline void xga() { setFromName("xga"); }

        inline void hd() { setFromName("hd"); }

        inline void sxga() { setFromName("sxga"); }

        inline void uxga() { setFromName("uxga"); }

        inline void fhd() { setFromName("fhd"); }

        inline void p_hd() { setFromName("p_hd"); }

        inline void p_3mp() { setFromName("p_3mp"); }

        inline void qxga() { setFromName("qxga"); }

        inline void qhd() { setFromName("qhd"); }

        inline void wqxga() { setFromName("wqxga"); }

        inline void p_fhd() { setFromName("p_fhd"); }

        inline void qsxga() { setFromName("qsxga"); }

        inline void pi() { setFromName("pi"); }

    protected:
        String name;
    };
}