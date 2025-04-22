#ifndef ELOQUENTESP32_INCLUDE_STATUS_H
#define ELOQUENTESP32_INCLUDE_STATUS_H

#include <Arduino.h>

/**
 * Success or error status
 */
class Status {
public:
    String reason;

    /**
     * Constructor
     */
    Status() : reason("") {

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
    Status &fail(String reason) {
        this->reason = reason;

        if (reason != "")
            ESP_LOGE("espkit", "%s", reason.c_str());

        return *this;
    }

    /**
     * Mark status as success
     * @return
     */
    Status &succeed() {
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
};

#endif //ELOQUENTESP32_INCLUDE_STATUS_H
