#pragma once

#include "../../networking/wifikit/WiFiKit.h"
#include "../../networking/http/Request.h"
#include "../../networking/http/Response.h"
#include "../../networking/http/AssetResponse.h"
#include "../../assets/CameraWebServer.h"
#include "./MjpegWebServer.h"

using espkit::networking::http::HttpServer;
using espkit::networking::http::Request;
using espkit::networking::http::Response;
using espkit::networking::http::AssetResponse;
using espkit::assets::Asset;


namespace cam {
    /**
     * "Classic" camera web server with sensor controls,
     * eloquent style
     */
    class CameraWebServer : public HttpServer {
    public:
        /**
         *
         */
        CameraWebServer() {
            listenOn(80);
            html = &(espkit::assets::CameraWebServer);
        }

        /**
         * Init server
         * @return
         */
        Status &begin() {
            mjpegServer.listenOn(config.port + 1);
            server.begin(config.port);
            mjpegServer.begin();
            setEventHandler<CameraWebServer>();
            ESP_LOGI("HTTP", "Camera Web Server address: http://%s:%d", wifikit.getIP().c_str(), config.port);

            return succeed();
        }

        /**
         * Handle request
         * @param request
         */
        virtual void handleRequest(Request *request) {
            if (request->path == "/") {
                AssetResponse response(request, html);

                response.send();
            } else if (request->path == "/configure") {
                Response response(request);
                const String key = request->getQueryParam("k");
                const String val = request->getQueryParam("v");
                const String update = key + "=" + val;

                camera.sensor.set(update);
                response.ok(update);
            }
        }

    protected:
        Asset *html;

        /**
         *
         * @return
         */
        String name() const {
            return "CameraWebServer";
        }
    };
}

static cam::CameraWebServer cameraWebServer;