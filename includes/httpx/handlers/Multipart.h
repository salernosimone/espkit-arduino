#pragma once

#include <WiFi.h>

namespace esptoolkit::internals {
    /**
     * Send multipart body to request
     * (only one part is supported)
     */
    class HttpxMultipart : public Stream {
    public:
        size_t size;

        /**
         * Empty body
         */
        HttpxMultipart(String header, String boundary, uint8_t *d, size_t s)
                : opening(String("--") + boundary + "\r\n" + header + "\r\n\r\n"),
                  closing(String("\r\n--") + boundary + "--\r\n"),
                  data(d),
                  length(s) {
            limits.pos = 0;
            limits.startOfData = opening.length();
            limits.endOfData = limits.startOfData + length;
            size = limits.endOfData + closing.length();
        }

        /**
         * Apply
         */
        void operator()(NetworkClientSecure *networkClient, HTTPClient *httpClient) const {
            // do nothing, this is handled in httpx
        }

        /**
         * Get available bytes
         * @return
         */
        int available() {
            ESP_LOGI("Multipart", "available=%d", size - limits.pos);
            return size - limits.pos;
        }

        /**
         * Stream API
         * (not used)
         * @param x
         * @return
         */
        size_t write(uint8_t x) {
            return 0;
        }

        /**
         * Stream API
         * (not used)
         * @return
         */
        int read() {
            // read opening
            if (limits.pos < limits.startOfData) {
                limits.pos += 1;

                return opening.charAt(limits.pos - 1);
            }

            // read closing
            if (limits.pos >= limits.endOfData) {
                limits.pos += 1;

                return closing.charAt(limits.pos - limits.endOfData - 1);
            }

            // read data
            const size_t i = limits.pos - limits.startOfData;
            limits.pos += 1;

            return data[i];
        }

        /**
         * Stream API
         * (not used)
         * @return
         */
        int peek() {
            return 0;
        }

    protected:
        uint8_t *data;
        size_t length;
        String opening;
        String closing;
        struct {
            size_t pos;
            size_t startOfData;
            size_t endOfData;
        } limits;
    };
}