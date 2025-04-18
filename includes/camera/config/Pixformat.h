#ifndef ELOQUENTESP32_INCLUDE_PIXFORMAT_H
#define ELOQUENTESP32_INCLUDE_PIXFORMAT_H

#include <esp_camera.h>

namespace espkit::cam {
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
         *
         */
        void jpeg() {
            ESP_LOGD("Camera.Config", "JPEG pixformat enabled");
            format = PIXFORMAT_JPEG;
        }

        /**
         *
         */
        void gray() {
            ESP_LOGD("Camera.Config", "Grayscale pixformat enabled");
            format = PIXFORMAT_GRAYSCALE;
        }

        /**
         *
         */
        void rgb() {
            ESP_LOGD("Camera.Config", "RGB 565 pixformat enabled");
            format = PIXFORMAT_RGB565;
        }

        /**
         *
         */
        void raw() {
            ESP_LOGD("Camera.Config", "RGB 888 pixformat enabled");
            format = PIXFORMAT_RGB888;
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_PIXFORMAT_H
