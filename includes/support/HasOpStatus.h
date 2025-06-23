#pragma once

#include "./Str.h"
#include "./OpStatus.h"

namespace esptoolkit {
    /**
     * Trait for classes that have a result status
     * (success or error)
     */
    class HasOpStatus {
    public:
        OpStatus opStatus;

        /**
         * Convert to bool
         * @return
         */
        explicit virtual operator bool() const {
            return (bool) (opStatus);
        }

        /**
         * Get reason of failure
         * @return
         */
        inline String reason() const {
            return opStatus.reason;
        }

        /**
         * Enter a endless loop of debug messages
         */
        void raise() {
            while (opStatus.failed()) {
                Serial.println(opStatus.reason);
                delay(1000);
            }
        }

        /**
         * Short syntax
         * @return
         */
        OpStatus &succeed() {
            return opStatus.succeed();
        }

        /**
         * Set fail reason
         * @param message
         * @return
         */
        OpStatus &fail(String message) {
            return opStatus.fail(message);
        }

        /**
         * Set fail reason
         * @param message
         * @return
         */
        OpStatus &fail(Str &message) {
            return fail(message.toString());
        }

        /**
         * Set fail reason with error code
         * @param message
         * @param errorCode
         * @return
         */
        OpStatus &failWithCode(const String &message, const int errorCode) {
            return opStatus.failWithCode(message, errorCode);
        }

        /**
         * Short syntax
         * @return
         */
        bool failed() const {
            return opStatus.failed();
        }

        /**
         * Short syntax
         * @return
         */
        bool succeeded() const {
            return !failed();
        }
    };
}