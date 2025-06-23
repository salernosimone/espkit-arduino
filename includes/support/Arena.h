#pragma once

namespace espkit::support {
    /**
     * Re-allocatable memory buffer
     */
    class Arena {
    public:
        uint8_t *data;
        size_t size;

        /**
         * Constructor
         */
        Arena() : data(NULL), size(0) {

        }

        /**
         * Destructor
         */
        ~Arena() {
            free(data);
        }

        /**
         * Allocate memory
         * @param s
         */
        bool alloc(size_t s) {
            // first allocation
            if (size == 0) {
                ESP_LOGD("Arena", "Allocating %d bytes of memory", s);
                data = (uint8_t *) ps_malloc(s);
            }
                // re-allocation
            else if (s != size) {
                ESP_LOGD("Arena", "(Re)Allocating %d bytes of memory (was %d)", s, size);
                data = (uint8_t *) ps_realloc(data, s);
            }

            size = s;

            return data != NULL;
        }

        /**
         * Set all elements to zero
         */
        void zero() {
            memset(data, 0, size);
        }

    protected:

    };
}