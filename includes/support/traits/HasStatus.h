#ifndef ELOQUENTESP32_INCLUDE_HASSTATUS_H
#define ELOQUENTESP32_INCLUDE_HASSTATUS_H

#include "../Str.h"
#include "../Status.h"

namespace espkit::support {
    /**
     * Trait for classes that have a result status
     * (success or error)
     */
    class HasStatus {
    public:
        Status status;

        /**
         * Convert to bool
         * @return
         */
        virtual operator bool() const {
            return (bool) (status);
        }

        /**
         * Get reason of failure
         * @return
         */
        inline String reason() const {
            return status.reason;
        }

        /**
         * Enter a endless loop of debug messages
         */
        void raise() {
            while (status.failed()) {
                Serial.println(status.reason);
                delay(1000);
            }
        }

        /**
         * Short syntax
         * @return
         */
        Status &succeed() {
            return status.succeed();
        }

        /**
         * Set fail reason
         * @param message
         * @return
         */
        Status &fail(String message) {
            return status.fail(message);
        }

        /**
         * Set fail reason
         * @param message
         * @return
         */
        Status &fail(Str &message) {
            return fail(message.toString());
        }

        /**
         * Set fail reason with error code
         * @param message
         * @param errorCode
         * @return
         */
        Status &failWithCode(const String &message, const int errorCode) {
            return fail(message + " (error code " + errorCode + ")");
        }

        /**
         * Short syntax
         * @return
         */
        bool failed() const {
            return status.failed();
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

#endif //ELOQUENTESP32_INCLUDE_HASSTATUS_H
