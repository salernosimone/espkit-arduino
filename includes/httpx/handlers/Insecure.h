#pragma once

#include <NetworkClientSecure.h>
#include <HTTPClient.h>

namespace esptoolkit::internals {
    /**
     * Ignore SSL cert verification
     */
    class HttpxInsecure {
    public:

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            networkClient->setInsecure();
        }
    };
}