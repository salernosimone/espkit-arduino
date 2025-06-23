#pragma once

#include <NetworkClientSecure.h>
#include <HTTPClient.h>

namespace esptoolkit::internals {
    /**
     * Add SSL certificate to request
     */
    class HttpxCert {
    public:
        const char *cert;

        /**
         * Constructor
         * @param c
         */
        HttpxCert(const char *c) : cert(c) {

        }

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            networkClient->setCACert(cert);
        }
    };
}