#pragma once

#include "../../networking/wifikit/WiFiKit.h"
#include "../../networking/http/HttpServer.h"
#include "../../networking/http/AssetResponse.h"
#include "../../networking/http/MultipartResponse.h"
#include "../../assets/MjpegWebServer.h"
#include "../Camera.h"

using espkit::networking::http::HttpServer;
using espkit::networking::http::Request;
using espkit::networking::http::AssetResponse;
using espkit::networking::http::MultipartResponse;


namespace espkit::cam {
    /**
     * Mjpeg HTTP server
     */
    class MjpegWebServer : public HttpServer {
    public:

        /**
         *
         */
        MjpegWebServer() {
            listenOn(80);
            html = &(espkit::assets::MjpegWebServer);
        }

        /**
         * Init server
         * @return
         */
        Status &begin() {
            server.begin(config.port);
            setEventHandler<MjpegWebServer>();
            ESP_LOGI("HTTP", "MJPEG server address: http://%s:%d", wifikit.getIP().c_str(), config.port);

            return succeed();
        }

        /**
         * Handle request
         * @param request
         */
        void handleRequest(Request *request) {
            // on /, return html page
            if (request->path == "/") {
                AssetResponse response(request, html);

                response.send();
            } else {
                // on every other path, return stream
                MultipartResponse response = MultipartResponse(request);

                while (request->client->connected()) {
                    auto frame = camera.grab();

                    if (frame) {
                        //camera.queue();
                        response.frame(frame.u8(), frame.size(), "image/jpeg");
                    } else {
                        ESP_LOGE("MjpegWebServer", "invalid frame");
                    }

                    delay(5);
                }
            }

            request->client->stop();
        }

    protected:
        Asset *html;

        /**
         *
         * @return
         */
        String name() const {
            return "MJPEG";
        }
    };
}

static espkit::cam::MjpegWebServer mjpegServer;
