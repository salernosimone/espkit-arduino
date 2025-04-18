#ifndef ELOQUENTESP32_INCLUDE_QUALITY_H
#define ELOQUENTESP32_INCLUDE_QUALITY_H

namespace espkit::cam {
    /**
     * Jpeg quality
     */
    class Quality {
    public:
        uint8_t quality;

        /**
         * Constructor
         */
        Quality() {
            high();
        }

        /**
         * Set quality from 0 (lowest) to 100 (highest)
         * @param q
         */
        void set(uint8_t q) {
            ESP_LOGD("Camera.Config", "Set quality to %d", q);
            quality = constrain(map((float) q, 0, 100, 64, 10), 10, 64);
        }

        /**
         * Set quality from string
         * @param q
         */
        void set(String q) {
            if (q == "best")
                best();
            else if (q == "high")
                high();
            else if (q == "mid")
                mid();
            else if (q == "low")
                low();
            else if (q == "worst")
                worst();
            else {
                // try to parse as number
                int8_t value = q.toInt();

                if (value > 0)
                    set(value);
                else ESP_LOGE("Quality", "Can't parse quality setting: %s", q.c_str());
            }
        }

        /**
         *
         */
        void worst() {
            set((uint8_t) 0);
        }

        /**
         *
         */
        void low() {
            set((uint8_t) 30);
        }

        /**
         *
         */
        void mid() {
            set((uint8_t) 50);
        }

        /**
         *
         */
        void high() {
            set((uint8_t) 80);
        }

        /**
         *
         */
        void best() {
            set((uint8_t) 100);
        }
    };
}

#endif //ELOQUENTESP32_INCLUDE_QUALITY_H
