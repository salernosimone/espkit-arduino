#pragma once

#include <functional>

namespace esptoolkit::threads {
    /**
     * Type definition for task
     * (function that accepts void* and returns void)
     */
    using Task = std::function<void(void *)>;
}
