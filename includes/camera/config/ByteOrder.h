#pragma once

namespace espkit::cam::config {
    /**
     * Handle RGB565 byte order
     */
    class ByteOrder {
    public:
        bool shouldSwap;

        /**
         *
         */
        ByteOrder() : shouldSwap(false) {

        }

        /**
         * Enable/disable swapping
         * @param enabled
         */
        inline void swap(const bool enabled) {
            shouldSwap = enabled;
        }

        /**
         *
         * @param order
         */
        void set(const String &order) {
            if (order == "idf")
                swap(false);
            else if (order == "natural")
                swap(false);
            else ESP_LOGE("Camera.Config.ByteOrder", "Unknown order: %s", String(order).c_str());
        }
    };
}