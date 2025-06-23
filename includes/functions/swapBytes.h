#pragma once

namespace espkit::functions {
    /**
     * Swap uint16_t bytes order
     * @param bytes
     * @param count
     */
    void swapBytes(uint16_t *bytes, size_t count) {
        if (bytes == NULL || count == 0)
            return;

        for (size_t i = 0; i < count; i += 2) {
            // ~15% faster than single instruction on 240x240
            bytes[i] = __builtin_bswap16(bytes[i]);
            bytes[i + 1] = __builtin_bswap16(bytes[i + 1]);
        }
    }
}