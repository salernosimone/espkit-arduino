#pragma once

#include <Arduino.h>

namespace espkit::support {
    /**
     * A simple dictionary
     */
    template<typename Key, typename Value, uint8_t size>
    class Dict {
    public:
        Key keys[size];
        Value values[size];

        /**
         * Constructor
         */
        Dict() : head(0) {

        }

        /**
         * Add key/value
         * @param key
         * @param value
         * @return
         */
        bool add(Key key, Value value) {
            if (head == size)
                return false;

            keys[head] = key;
            values[head] = value;
            head += 1;

            return true;
        }

        /**
         * Get value by key
         * @param key
         * @param defaultValue
         * @return
         */
        Value get(Key key, Value defaultValue) {
            for (uint8_t i = 0; i < head; i++)
                if (keys[i] == key)
                    return values[i];

            return defaultValue;
        }

    protected:
        uint8_t head;
    };
}