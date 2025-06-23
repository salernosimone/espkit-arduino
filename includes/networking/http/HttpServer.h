#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "../../idf/xCore.h"
#include "../../support/HasOpStatus.h"
#include "./Request.h"
#include "./Response.h"

using espkit::idf::xCore;

namespace espkit::networking::http {
    /**
     * Http server base class.
     * It is meant to be overriden
     */
    class HttpServer : public HasStatus {
    public:
        ::NetworkServer server;

        /**
         * Init server
         * @return
         */
        virtual Status &begin() = 0;

        /**
         * Handle request
         * @param request
         */
        virtual void handleRequest(Request *request) = 0;

        /**
         * Set port
         * @param port
         */
        void listenOn(uint16_t port) {
            config.port = port;
        }

        /**
         * Handle new clients
         */
        template<typename HttpdImpl>
        void setEventHandler() {
            core0.config.userdata((void *) this);
            core0.config.stack(stackSize());
            core0.config.priority(priority());
            core0.pin(name(), [](void *userdata) {
                HttpdImpl *handler = (HttpdImpl *) userdata;

                while (true) {
                    NetworkClient client = handler->server.accept();
                    Request request(&client);

                    if (!client || !client.available()) {
                        delay(10);
                        continue;
                    }

                    request.parseHeadline();

                    if (request.method.length() < 3) {
                        ESP_LOGE("Httpd", "Malformed request");
                        continue;
                    }

                    ESP_LOGD("Httpd", "Handling %s request to %s%s", request.method, request.path, request.queryString);
                    handler->handleRequest(&request);
                    request.ignore();
                    ESP_LOGD("Httpd", "Handled %s request to %s%s", request.method, request.path, request.queryString);
                }
            });
        }

    protected:
        struct {
            uint16_t port = 80;
        } config;

        /**
         * Get task name
         * @return
         */
        virtual String name() const = 0;

        /**
         * Get task stack size
         * @return
         */
        virtual uint16_t stackSize() const {
            return 5000;
        }

        /**
         * Get task priority
         * @return
         */
        virtual uint8_t priority() const {
            return 5;
        }

        /**
         *
         * @tparam Impl
         * @return
         */
        template<typename Impl>
        Status &beginT() {
            server.begin(config.port);

            return succeed();

            //        core0.pin(name(), [](void *userdata) {
            //            Impl *httpd = (Impl *) userdata;
            //            NetworkClient client;
            //
            //            while (true) {
            //                delay(10);
            //                client = httpd->server.accept();
            //
            //                if (client && client.available()) {
            //                    ESP_LOGI("httpd", "new client connected!");
            //                    Request request(&client);
            //
            //                    request.parseHeadline();
            //
            //                    if (request.method.length() < 3) {
            //                        ESP_LOGE("Httpd", "Bad request headline");
            //                        continue;
            //                    }
            //
            //                    ESP_LOGI("Httpd", "Handling %s request to %s%s", request.method, request.path, request.queryString);
            ////                    httpd->handle(&request);
            //
            //                    // handle request in a new thread
            //                    httpd->setRequest(&request);
            //                    core0.pin(str::concat("httpdHandle", millis()), [](void *userdata) {
            //                        Impl *httpd = (Impl *) userdata;
            //                        httpd->handle();
            //                        httpd->request->ignore();
            //                        vTaskDelete(NULL);
            //                    }, "userdata", userdata);
            //                }
            //            }
            //        }, "stackSize", stackSize(), "priority", priority(), "userdata", (void *) this);
            //
            //        return succeed();
        }
    };
}