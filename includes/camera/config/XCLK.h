#ifndef ELOQUENTESP32_INCLUDE_XCLK_H
#define ELOQUENTESP32_INCLUDE_XCLK_H

namespace espkit::cam {
    /**
     * Set clock speed
     */
    class XCLK {
    public:
        uint32_t freq;

        /**
         * Constructor
         */
        XCLK() {
            fast();
        }

        /**
         * Set fast speed
         */
        void fast() {
            ESP_LOGD("Camera.Config", "Fast clock (20 Mhz) enabled");
            freq = 20000000ULL;
        }

        /**
         * Set slow speed
         */
        void slow() {
            ESP_LOGD("Camera.Config", "Slow clock (10 MHz) enabled");
            freq = 10000000ULL;
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_XCLK_H
