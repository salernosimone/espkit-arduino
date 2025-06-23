#pragma once

#include <Arduino.h>

namespace esptoolkit {
    /**
     * Operation result status
     * (either success or error)
     */
    class OpStatus {
    public:
        String reason;

        /**
         * Constructor
         */
        OpStatus() : reason("") {

        }

        /**
         * Convert to boolean
         * @return
         */
        operator bool() const {
            return succeeded();
        }

        /**
         * Set fail reason
         * @param reason
         * @return
         */
        OpStatus &fail(String reason) {
            this->reason = reason;

            if (reason != "")
                ESP_LOGE("esptoolkit", "%s", reason.c_str());

            return *this;
        }

        /**
         * Set fail reason with error code
         * @param message
         * @param errorCode
         * @return
         */
        OpStatus &failWithCode(const String &message, const int errorCode) {
            return fail(message + " (error code " + errorCode + ")");
        }

        /**
         * Mark status as success
         * @return
         */
        OpStatus &succeed() {
            return fail("");
        }

        /**
         *
         * @return
         */
        bool failed() const {
            return reason != "";
        }

        /**
         *
         * @return
         */
        bool succeeded() const {
            return !failed();
        }

        /**
         * If failed, loop endlessly
         */
        void raise() const {
            while (failed()) {
                Serial.println(reason);
                delay(1000);
            }
        }
    };
}