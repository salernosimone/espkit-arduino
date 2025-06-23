#pragma once

#include <HTTPClient.h>
#include "../support/HasOpStatus.h"
#include "../support/URL.h"

using esptoolkit::OpStatus;
using esptoolkit::HasOpStatus;
using esptoolkit::internals::URL;

namespace esptoolkit {
    /**
     * Httpx response
     */
    class HttpxResponse : public HasOpStatus {
    public:
        int code;
        HTTPClient *httpClient;

        /**
         * Constructor
         */
        HttpxResponse() : code(0), httpClient(NULL) {

        }

        /**
         * Convert to bool
         * @return
         */
        explicit virtual operator bool() const {
            return (bool) (opStatus) && (code >= 100 && code < 400);
        }

        /**
         *
         * @param url
         * @return
         */
        HttpxResponse &connectionFailed(const String &url) {
            URL u(url);

            fail(String("Failed to connect to ") + u.host);

            return *this;
        }

        /**
         *
         * @param client
         * @param code
         * @return
         */
        HttpxResponse &pipe(HTTPClient &client, int code) {
            this->code = code;
            httpClient = &client;

            if (code <= 0)
                fail(client.errorToString(code));
            else succeed();

            return *this;
        }

        /**
         * Get body as text
         * @return
         */
        String text() {
            return httpClient == NULL ? "NULL" : httpClient->getString();
        }

    };
}