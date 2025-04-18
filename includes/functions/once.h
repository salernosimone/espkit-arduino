#pragma once

#include <Arduino.h>
#include "../constants.h"

// define once function as macro
// to automatically generate a unique id
#define once(callback) espkit::internals::onceInternal(callback, (uint32_t)(__LINE__ * 2654435761UL ^ __COUNTER__))

uint32_t executedIds[MAX_ONCE_CALLS] = {0};
uint16_t executedCount = 0;

namespace espkit::internals {
    /**
     * Run function once
     * @tparam Callback
     * @param callback
     * @param id
     */
    template<typename Callback>
    void onceInternal(Callback callback, uint32_t id) {
        // return if already called
        for (uint16_t i = 0; i < MAX_ONCE_CALLS; i++)
            if (executedIds[i] == id)
                return;

        // return if no more space for calling
        if (executedCount >= MAX_ONCE_CALLS)
            return;

        executedIds[executedCount++] = id;
        callback();
    }
}