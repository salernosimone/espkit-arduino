#pragma once

#include "./Config.h"

namespace esptoolkit::threads {
    /**
     * Configure thread user data pointer
     */
    class UserData {
    public:
        /**
         * Constructor
         * @param userdata
         */
        UserData(void *userdata) : data(userdata) {

        }

        /**
         * Apply config
         * @param config
         */
        void operator()(Config *config) {
            config->userdata = data;
        }

    protected:
        void *data;
    };
}