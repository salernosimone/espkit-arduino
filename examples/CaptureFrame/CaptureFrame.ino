/**
 * Get started with the EloquentEsp32 library.
 * Configure camera and grab frames.
 *
 * Turn on INFO logging in the Tools menu to see
 * debug messages.
 *
 * Rembember to enable PSRAM in the Tools menu!
 */
#include <espkit.h>
#include <espkit/camera.h>


void setup() {
    delay(3000);
    Serial.begin(115200);
    ESP_LOGI("APP", "Start");

    // set camera pins
    // let user choose from a list (open Serial Monitor!)
    // after you choose the pinout, a line will be printed
    // to avoid choosing again in the future
    // e.g. `camera.config.pinout.aithinker()`
    camera.config.pinout.prompt();
    // ...or, if you don't know which model to select
    // you can try to guess
    // (WARNING: will probably cause reboots until a match is found!)
    camera.config.pinout.guess();

    // enable JPEG mode
    // alternatives are:
    // - gray() for grayscale mode
    // - rgb() for RGB565 mode
    // - raw() for RGB888
    // NOTE: not all boards support all modes!
    camera.config.pixformat.jpeg();

    // when in JPEG mode, you can set the quality
    // of the image. Higher quality requires
    // more memory.
    // alternatives are:
    // - best() for highest quality
    // - mid()
    // - low()
    // - worst()
    // - set(0..100) to set a custom quality
    //   where 0 is worst and 100 is best
    camera.config.quality.high();

    // set frame resolution
    // choosing from a list of options
    //camera.config.resolution.prompt();
    // or set directly
    camera.config.resolution.qvga();

    // if you prefer, you can set all config values using a string
    // the options above translate to the following:
    //camera.config("format:jpeg q:high res:square pinout:prompt");
    // init camera
    // will return "falsy" if something goes wrong
    camera.begin();
    // if something went wrong, abort execution
    // (will print the error message forever)
    camera.raise();
}

void loop() {
    // grab new frame
    // will return "falsy" if something goes wrong
    auto frame = camera.grab();

    if (!frame) {
        Serial.println(frame.reason());
        delay(1000);
        return;
    }

    // print info about the frame
    // frame.size() returns the size in bytes of the frame
    // frame.fb gives access to the underlying camera_fb_t*
    ESP_LOGI(
            "App",
            "Frame size: %d bytes, t=%d",
            frame.size(),
            frame.fb->timestamp.tv_usec / 1000
    );

    delay(1000);
}
