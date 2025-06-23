#pragma once

#include <NetworkClientSecure.h>
#include <HTTPClient.h>

namespace esptoolkit::internals {
    /**
     * Generic header
     */
    class HttpxHeader {
    public:
        const char *name;
        const char *value;

        /**
         *
         * @param k
         * @param v
         */
        HttpxHeader(const char *k, const String &v) : name(k), value(v.c_str()) {

        }

        /**
         *
         * @param k
         * @param v
         */
        HttpxHeader(const String &k, const String &v) : name(k.c_str()), value(v.c_str()) {

        }

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            httpClient->addHeader(name, value);
        }
    };
}