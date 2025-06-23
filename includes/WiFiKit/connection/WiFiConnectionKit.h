#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "../../support/Str.h"
#include "../../support/HasOpStatus.h"
#include "./config/Config.h"
#include "./config/Timeout.h"
#include "./config/Retry.h"

using esptoolkit::Str;
using esptoolkit::wifi::connection::Config;
using TimeoutType = esptoolkit::wifi::connection::Timeout;
using RetryType = esptoolkit::wifi::connection::Retry;

String wifiConnDisconnectReason("");

namespace esptoolkit {
    /**
     * Utilities to connect to WiFi
     */
    class WiFiConnectionKit : public HasOpStatus {
    public:
        String ip;
        Config config;

        /**
         * Constructor
         */
        WiFiConnectionKit() : ip(""), connectedSSID("") {
        }

        /**
         * Short for isConnected.
         * Allows `if (wifiConn)` syntax
         * @return
         */
        explicit operator bool() const {
            return !!opStatus && isConnected();
        }

        /**
         * Try to connect
         * @param ssid
         * @param pass
         * @return
         */
        OpStatus &operator()(const String &ssid, const String &pass) {
            return connect(ssid.c_str(), pass.c_str(), Timeout("8s"), Retry(0));
        }

        /**
         * Try to connect
         * @param ssid
         * @param pass
         * @return
         */
        template<typename T, typename... KWArgs>
        OpStatus &operator()(const String &ssid, const String &pass, T arg, KWArgs... kwargs) {
            return connect(ssid.c_str(), pass.c_str(), arg, kwargs...);
        }

        /**
         * Check if board is connected to WiFi
         * @return
         */
        inline bool isConnected() const {
            return WiFi.status() == WL_CONNECTED;
        }

        /**
         * Factory method for timeout config
         * @param timeout
         * @return
         */
        TimeoutType Timeout(const size_t timeout) const {
            return TimeoutType(timeout);
        }

        /**
         * Factory method for timeout config
         * @param timeout
         * @return
         */
        TimeoutType Timeout(const String &timeout) const {
            return TimeoutType(timeout);
        }

        /**
         * Factory method for retries config
         * @param retries
         * @return
         */
        RetryType Retry(const uint8_t retries) const {
            return RetryType(retries);
        }

    protected:
        String connectedSSID;

        /**
         * Try to connect to WiFi
         * @return
         */
        OpStatus &connect(const char *ssid, const char *pass) {
            // don't reconnect if already connected to same SSID
            if (isConnected() && connectedSSID == String(ssid))
                return opStatus;

            ip = "";
            WiFi.mode(WIFI_STA);
            WiFi.disconnect();
            listen();

            for (uint8_t i = 0; i <= config.retries && !isConnected(); i++) {
                WiFi.begin(ssid, pass);
                const size_t timeout = config.timeout + millis() + 1;

                while (millis() < timeout && WiFi.status() != WL_CONNECTED)
                    delay(100);
            }

            // if connection succeeded, get IP address as string
            if (isConnected()) {
                IPAddress addr = WiFi.localIP();

                connectedSSID = ssid;
                ip = Str::from(addr[0]).join(".", addr[1], addr[2], addr[3]).toString();

                return succeed();
            }

            return fail(wifiConnDisconnectReason);
        }

        /**
         * Consume arguments
         * @tparam T
         * @tparam KWArgs
         * @param ssid
         * @param pass
         * @param arg
         * @param kwargs
         * @return
         */
        template<typename T, typename... KWArgs>
        OpStatus &connect(const char *ssid, const char *pass, T arg, KWArgs... kwargs) {
            arg(&config);

            return connect(ssid, pass, kwargs...);
        }

        /**
         *
         */
        void listen() {
            static bool registered = false;

            if (!registered) {
                registered = true;

                // get detailed error status
                WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
                    switch (info.wifi_sta_disconnected.reason) {
                        case WIFI_REASON_NO_AP_FOUND:
                            wifiConnDisconnectReason = "AP not found";
                            break;
                        case WIFI_REASON_AUTH_FAIL:
                            wifiConnDisconnectReason = "Wrong WiFi password";
                            break;
                        case WIFI_REASON_HANDSHAKE_TIMEOUT:
                        case WIFI_REASON_BEACON_TIMEOUT:
                            wifiConnDisconnectReason = "Handshake/Beacon timeout";
                            break;
                        default:
                            wifiConnDisconnectReason = "Unknown WiFi connection error";
                    }
                }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
            }
        }

    };
}

// global singleton
static esptoolkit::WiFiConnectionKit wifiConn;
