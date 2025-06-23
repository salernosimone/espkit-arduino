#pragma once

#include <WiFi.h>
#include "../../support/JSONStr.h"

namespace esptoolkit::internals {
    /**
     * Send body to request
     * (may be empty)
     */
    class HttpxBody {
    public:
        const String *text;
        uint8_t *buffer;
        size_t length;

        /**
         * Empty body
         */
        HttpxBody() : length(0), text(NULL), buffer(NULL) {

        }

        /**
         *
         * @param t
         */
        HttpxBody(const char *t) : s(t), text(&s) {

        }

        /**
         * Text body
         * @param t
         */
        HttpxBody(const String &t) : length(t.length()), text(&t), buffer(NULL) {

        }

        /**
         * Text body
         * @param b
         * @param len
         */
        HttpxBody(uint8_t *b, size_t len) : length(len), text(NULL), buffer(b) {

        }

        /**
         *
         * @param json
         */
        HttpxBody(JSONStr &json) : HttpxBody(json.toString().c_str()) {

        }

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            // do nothing, this is handled in httpx
        }

    protected:
        String s;
    };
}