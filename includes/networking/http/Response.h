#pragma once

#include <functional>
#include <Arduino.h>
#include <WiFi.h>
#include "../../../ignore/deprecated/str.h"
#include "./Request.h"

namespace espkit::networking::http {
    /**
     *
     */
    class Response {
    public:
        Request *request;

        /**
         * Constructor
         * @param request_
         */
        Response(Request *request_) :
                request(request_),
                beforeBodyCallback(NULL) {

        }

        /**
         * Send ok status
         */
        void ok() {
            println(str::join(" ", "HTTP/1.1", 200, "OK"));
        }

        /**
         * Send ok status
         */
        void ok(const String &message) {
            ok();
            plain(message);
        }

        /**
         * Send server error status
         */
        void serverError(const String &message) {
            println(str::join(" ", "HTTP/1.1", 500, "SERVER ERROR"));
            plain(message);
        }

        /**
         * Send 404 status
         */
        void notFound() {
            println(str::join(" ", "HTTP/1.1", 404, "Not Found"));
        }

        /**
         * Print header
         * @tparam T
         * @param name
         * @param value
         */
        template<typename T>
        void header(String name, T value) {
            println(str::concat(name, ": ", value));
        }

        /**
         * Print header
         * @tparam T
         * @param name
         * @param value
         */
        void header(String name, const char *value) {
            header(name, String(value));
        }

        /**
         * Enable CORS requests
         */
        void cors() {
            header("Access-Control-Allow-Origin", "*");
        }

        /**
         * Return plain text
         * @param text
         */
        void plain(const String &text) {
            header("Content-Type", "text/plain");
            body(text);
        }

        /**
         * Send JPEG image
         * @param data
         * @param length
         */
        void jpeg(uint8_t *data, const size_t length) {
            header("Content-Type", "image/jpeg");
            body(data, length);
        }

        /**
         * Send string body
         * @param body
         */
        void body(const String &body) {
            header("Content-Length", body.length());
            beforeBody();
            print(body);
        }

        /**
         * Send binary body
         * @param body
         */
        void body(uint8_t *body, const size_t length) {
            header("Content-Length", length);
            beforeBody();
            request->client->write(body, length);
        }

        /**
         * Start on-demand body
         */
        void body() {
            beforeBody();
        }

        /**
         * Call function before sending body
         * @param callback 
         */
        void beforeBody(std::function<void(Response *)> callback) {
            beforeBodyCallback = callback;
        }

        /**
         * Print
         * @tparam T
         * @param value
         */
        template<typename T>
        void print(T value) {
            // flush pending request, if any
            request->ignore();
            request->client->print(value);
        }

        /**
         * Println
         * @tparam T
         * @param value
         */
        template<typename T>
        void println(T value) {
            print(value);
            println();
        }

        /**
         * Println
         */
        void println() {
            print("\r\n");
        }

    protected:
        std::function<void(Response *)> beforeBodyCallback;

        /**
         * Call before body callback
         */
        void beforeBody() {
            if (beforeBodyCallback != NULL)
                beforeBodyCallback(this);

            println();
        }
    };
}
