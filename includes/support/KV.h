#pragma once

#include <Arduino.h>

namespace espkit::support {
    /**
     * Key/Value pair.
     * Splits on "="
     */
    class KV {
    public:
        String key;
        String value;

        /**
         * Constructor
         * @param k
         * @param v
         */
        KV(const String &k, const String &v) : key(k), value(v) {

        }

        /**
         * Constructor
         * @param kv
         */
        KV(const String &kv) : key(""), value("") {
            const int8_t split = kv.indexOf("=");

            // no key
            if (split < 0) {
                value = kv;
            } else {
                key = kv.substring(0, split);
                value = kv.substring(split + 1);
            }
        }

        /**
         * Compare
         * @param k
         * @return
         */
        const bool operator==(const String &k) const {
            return key == k;
        }
    };
}