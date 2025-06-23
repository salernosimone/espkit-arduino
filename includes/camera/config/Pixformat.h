#ifndef ELOQUENTESP32_INCLUDE_PIXFORMAT_H
#define ELOQUENTESP32_INCLUDE_PIXFORMAT_H

#include <esp_camera.h>

namespace espkit::cam::config {
    /**
     * Pixel format (jpeg, rgb565, rgb888...)
     */
    class Pixformat {
    public:
        pixformat_t format;

        /**
         * Constructor
         */
        Pixformat() {
            jpeg();
        }

        /**
         * Set from name
         * @param format
         */
        void set(const String &format) {
            if (format == "jpeg") jpeg();
            else if (format == "gray") gray();
            else if (format == "rgb") rgb();
            else if (format == "raw") raw();
            else ESP_LOGE("Camera.Config.Pixformat", "Unknown format: %s", format.c_str());
        }

        /**
         *
         */
        void jpeg() {
            format = PIXFORMAT_JPEG;
        }

        /**
         *
         */
        void gray() {
            format = PIXFORMAT_GRAYSCALE;
        }

        /**
         *
         */
        void rgb() {
            format = PIXFORMAT_RGB565;
        }

        /**
         *
         */
        void raw() {
            format = PIXFORMAT_RGB888;
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_PIXFORMAT_H
