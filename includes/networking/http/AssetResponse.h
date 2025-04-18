#pragma once

#include "../../assets/Asset.h"
#include "./Response.h"

using espkit::assets::Asset;

namespace espkit::networking::http {
    /**
     * Wrap an asset into an HTTP response
     */
    class AssetResponse : public Response {
    public:

        /**
         *
         * @param request
         */
        AssetResponse(Request *request, Asset *asset_) :
                Response(request),
                asset(asset_) {

        }

        /**
         * Send response
         */
        void send() {
            if (asset->length == 0) {
                notFound();
                body("Asset not found");
                return;
            }

            ok();
            header("Content-Type", asset->contentType);
            header("Connection", "close");
            header("Content-Encoding", "gzip");
            body((uint8_t *) asset->data, asset->length);
        }

    protected:
        Asset *asset;
    };
}