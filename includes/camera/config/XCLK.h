#ifndef ELOQUENTESP32_INCLUDE_XCLK_H
#define ELOQUENTESP32_INCLUDE_XCLK_H

namespace espkit::cam::config {
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
         *
         * @param value
         */
        void set(const String &value) {
            if (value == "fast") fast();
            else if (value == "slow") slow();
            else ESP_LOGE("Camera.Config.XCLK", "Unkown speed: %s", String(value).c_str());
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
