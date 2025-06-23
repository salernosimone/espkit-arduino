#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include "./Response.h"
#include "./handlers/Cert.h"
#include "./handlers/Insecure.h"
#include "./handlers/Header.h"
#include "./handlers/Body.h"
#include "./handlers/Multipart.h"
#include "./handlers/ConnectTimeout.h"
#include "./handlers/RequestTimeout.h"

using esptoolkit::internals::HttpxBody;
using esptoolkit::internals::HttpxCert;
using esptoolkit::internals::HttpxInsecure;
using esptoolkit::internals::HttpxHeader;
using esptoolkit::internals::HttpxMultipart;
using esptoolkit::internals::HttpxConnectTimeout;
using esptoolkit::internals::HttpxRequestTimeout;


namespace esptoolkit {
    /**
     * Make HTTP requests
     */
    class Httpx {
    public:
        NetworkClientSecure networkClient;
        HTTPClient httpClient;
        HttpxResponse response;

        /**
         * Constructor
         */
        Httpx() : method(""), responseCode(0) {

        }


        /**
         * GET request
         * (without args)
         * @param url
         */
        HttpxResponse &get(const String &url) {
            return any("GET", url);
        }

        /**
         * POST request
         * (without args)
         * @param url
         */
        HttpxResponse &post(const String &url) {
            return any("POST", url);
        }

        /**
         * GET request
         * (with args)
         * @param url
         */
        template<typename... Args>
        HttpxResponse &get(const String &url, Args... args) {
            return any("GET", url, args...);
        }

        /**
         * POST request
         * (with args)
         * @param url
         */
        template<typename... Args>
        HttpxResponse &post(const String &url, Args... args) {
            return any("POST", url, args...);
        }

        /**
         *
         * @return
         */
        HttpxInsecure Insecure() {
            return HttpxInsecure();
        }

        /**
         *
         * @return
         */
        HttpxHeader Header(const String &name, const String &value) {
            return HttpxHeader(name, value);
        }

        /**
         *
         * @return
         */
        HttpxHeader Accept(const String &value) {
            return HttpxHeader("Accept", value);
        }

        /**
         *
         * @return
         */
        HttpxHeader ContentType(const String &value) {
            return HttpxHeader("Content-Type", value);
        }

        /**
         *
         * @return
         */
        HttpxConnectTimeout ConnectTimeout(const String &t) {
            return HttpxConnectTimeout(t);
        }

        /**
         *
         * @return
         */
        HttpxRequestTimeout RequestTimeout(const String &t) {
            return HttpxRequestTimeout(t);
        }

        /**
         *
         * @param cert
         * @return
         */
        HttpxCert Cert(const char *cert) {
            return HttpxCert(cert);
        }

        /**
         *
         * @param body
         * @return
         */
        HttpxBody Body(const String &body) {
            return HttpxBody(body);
        }

        /**
         *
         * @param data
         * @param length
         * @return
         */
        HttpxBody Body(uint8_t *data, size_t length) {
            return HttpxBody(data, length);
        }

        /**
         *
         * @param json
         * @return
         */
        HttpxBody Body(JSONStr json) {
            return HttpxBody(json);
        }

        /**
         *
         * @param header
         * @param boundary
         * @param data
         * @param length
         * @return
         */
        HttpxMultipart Multipart(String header, String boundary, uint8_t *data, size_t length) {
            return HttpxMultipart(header, boundary, data, length);
        }

    protected:
        String method;
        int responseCode;

        /**
         * Actual request implementation
         *
         * @tparam Args
         * @param method
         * @param url
         * @param args
         * @return
         */
        template<typename... Args>
        HttpxResponse &any(const String &method, const String &url, Args... args) {
            responseCode = 0;

            httpClient.end();

            if (!httpClient.begin(networkClient, url))
                return response.connectionFailed(url);

            this->method = method;

            // set defaults
            handle(
                    HttpxConnectTimeout("2s"),
                    HttpxRequestTimeout("30s")
            );

            // handle custom options
            handle(args...);

            // if request has not been sent yet, send now
            if (!responseCode) {
                responseCode = httpClient.sendRequest(method.c_str(), (uint8_t *) NULL, 0);
            }

            return response.pipe(httpClient, responseCode);
        }

        /**
         * Handle arg
         * (stop condition)
         */
        void handle() {

        }

        /**
         * Handle body (must be last!)
         * @param body
         */
        template<typename... Args>
        void handle(HttpxBody &body, Args... args) {
            if (body.buffer != NULL && body.length)
                responseCode = httpClient.sendRequest(method.c_str(), body.buffer, body.length);
            else
                responseCode = httpClient.sendRequest(method.c_str(), *(body.text));

            handle(args...);
        }

        /**
         * Handle multipart body (must be last!)
         * @param body
         */
        template<typename... Args>
        void handle(HttpxMultipart &body, Args... args) {
            ESP_LOGI("Httpx", "Multipart body: size=%d", body.size);
            responseCode = httpClient.sendRequest(method.c_str(), &body, body.size);

            handle(args...);
        }

        /**
         * Handle arg
         * @tparam T
         * @tparam Args
         * @param arg anything that can be called on a NetworkClient
         * @param args
         */
        template<typename T, typename... Args>
        void handle(T arg, Args... args) {
            arg(&networkClient, &httpClient);
            handle(args...);
        }
    };
}

static esptoolkit::Httpx httpx;