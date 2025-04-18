#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "../../support/traits/HasStatus.h"
#include "./WiFiKitConfig.h"

using espkit::support::HasStatus;

namespace espkit::networking {
    /**
     * Utilities to connect to WiFi
     */
    class WiFiKit : public HasStatus {
    public:
        String ip;
        WiFiKitConfig config;

        /**
         * Constructor
         */
        WiFiKit() : ip("") {

        }

        /**
         * Short for isConnected.
         * Allows if (wifikit) syntax
         * @return
         */
        explicit operator bool() const {
            return isConnected();
        }

        /**
         * Check if board is connected to WiFi
         * @return
         */
        inline bool isConnected() const {
            return WiFi.status() == WL_CONNECTED;
        }

        /**
         * Set SSID and password
         * @param ssid
         * @param password
         * @return
         */
        WiFiKit &to(const String &ssid, const String &password) {
            config.ssid(ssid);
            config.password(password);

            return *this;
        }

        /**
         * Try to connect to WiFi
         * @return
         */
        Status &connect() {
            for (uint8_t i = 0; !isConnected() && i < config.values.retries; i++)
                actualConnect();

            if (!isConnected()) {
                ip = "";

                return fail("Can't connect to WiFi. Check SSID and password");
            }

            getIP();

            return succeed();
        }

        /**
         * Get IP address as string
         * @return
         */
        String &getIP() {
            IPAddress addr = WiFi.localIP();

            ip = Str::from(addr[0]).join(".", addr[1], addr[2], addr[3]).toString();

            return ip;
        }

    protected:

        /**
         * Connection implementation
         */
        void actualConnect() {
            ESP_LOGD("wifikit", "Connecting to SSID=%s", config.values.ssid.c_str());

            if (WiFi.status() == WL_CONNECTED)
                return;

            WiFi.mode(WIFI_STA);
            WiFi.begin(config.values.ssid, config.values.password);

            const size_t timeout = config.values.timeout + millis() + 1;

            while (millis() < timeout && WiFi.status() != WL_CONNECTED)
                delay(100);
        }
    };
}

// global singleton
static espkit::networking::WiFiKit wifikit;
