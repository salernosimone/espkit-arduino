#pragma once

#include <Arduino.h>

namespace espkit::assets {
    /**
     * Base struct for an asset
     */
    struct Asset {
        const uint8_t *data;
        size_t length;
        String contentType;
    };
}