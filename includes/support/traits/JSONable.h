#pragma once

namespace espkit::support {
    /**
     * Interface for objects that can be converted to JSON
     */
    class JSONable {
    public:
        /**
         * Convert object to JSON
         * @return
         */
        virtual String &toJSON() = 0;
    };
}