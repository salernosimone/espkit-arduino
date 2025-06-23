#pragma once

#include <NetworkClientSecure.h>
#include <HTTPClient.h>
#include "../../support/Str.h"

using esptoolkit::Str;

namespace esptoolkit::internals {
    /**
     * Set connection timeout
     */
    class HttpxConnectTimeout {
    public:
        size_t timeout;

        /**
         *
         * @param t
         */
        HttpxConnectTimeout(const String &t) {
            timeout = Str::from(t).duration();
        }

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            httpClient->setConnectTimeout(timeout);
        }
    };
}