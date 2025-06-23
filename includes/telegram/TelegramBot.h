#pragma once

#include "../functions/encodeURIComponent.h"
#include "../support/Str.h"
#include "../support/HasOpStatus.h"
#include "../httpx/Httpx.h"

#define TELEGRAM_BOUNDARY "tlgrmbndry"

const char TELEGRAM_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh
MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE
YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3
MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo
ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg
MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN
ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA
PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w
wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi
EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY
avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+
YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE
sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h
/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5
IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj
YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD
ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy
OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P
TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ
HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER
dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf
ReYNnyicsbkqWletNw+vHX/bvZ8=
-----END CERTIFICATE-----
)EOF";

namespace esptoolkit {
    /**
     * Send text and pictures via Telegram
     */
    class TelegramBot : public HasOpStatus {
    public:

        /**
         *
         */
        TelegramBot() : cert(TELEGRAM_CERT), caption("") {
            token[0] = '\0';
        }

        /**
         *
         * @param token
         */
        void configure(const char *token) {
            strcpy(this->token, token);

            if (strlen(this->token) < 46)
                fail("Bad Telegram bot token");
        }

        /**
         * Set bot token
         * @param token
         */
        void configure(String &token) {
            configure(token.c_str());
        }

        /**
         * Set caption for picture
         * @param cap
         * @return
         */
        TelegramBot& withCaption(String cap) {
            caption = cap;

            return *this;
        }

        /**
         * Send text message
         * @param recipient
         * @param text
         * @return
         */
        OpStatus &sendText(String &recipient, String &text) {
            if (strlen(token) < 46)
                return fail("Bad Telegram bot token");

            String url = Str::from("https://api.telegram.org/bot")
                    .concat(token)
                    .concat("/sendMessage?chat_id=")
                    .concat(recipient)
                    .concat("&text=")
                    .concat(encodeURIComponent(text))
                    .toString();

            Httpx request;

            auto response = request.post(url, cert);

            if (!response)
                return failWithCode(response.reason(), response.code);

            return succeed();
        }

        /**
         *
         * @tparam Picture
         * @param recipient
         * @param picture
         * @param caption
         * @return
         */
        template<typename Picture>
        OpStatus &sendPicture(String& recipient, Picture& picture, String caption = "") {
            return sendPicture(recipient, picture.u8(), picture.size(), caption);
        }

        /**
         *
         * @param recipient
         * @param data
         * @param size
         * @param caption
         * @return
         */
        OpStatus &sendPicture(String& recipient, uint8_t *data, size_t length) {
            if (strlen(token) < 46)
                return fail("Bad Telegram bot token");

            String url = Str::from("https://api.telegram.org/bot")
                    .concat(token)
                    .concat("/sendPhoto?chat_id=")
                    .concat(recipient)
                    .concatIf(caption != "", String("&caption=") + encodeURIComponent(caption))
                    .toString();

            Httpx request;

            auto response = request.post(
                url,
                cert,
                httpx.ContentType("multipart/form-data; boundary=" TELEGRAM_BOUNDARY),
                httpx.Multipart("Content-disposition: form-data; name=\"photo\"; filename=\"esp32cam.jpg\"\r\nContent-Type: image/jpeg", TELEGRAM_BOUNDARY, data, length)
            );

            caption = "";

            if (!response)
                return failWithCode(response.text(), response.code);

            return succeed();
        }

    protected:
        char token[47];
        String caption;
        HttpxCert cert;
    };
}

static esptoolkit::TelegramBot telegramBot;