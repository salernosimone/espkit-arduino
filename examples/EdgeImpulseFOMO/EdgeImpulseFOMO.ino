/**
* Run Edge Impulse FOMO model on camera frames.
*/
// replace with your library name
#include <fomo-penguin-64x64x3.h>
#include <espkit.h>
#include <espkit/camera.h>
#include <espkit/edgeimpulse.h>


void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Edge Impulse FOMO example");

    // config camera
    // since we're using the RGB frame format
    // keep the resolution as low as possible
    // to save space.
    // yolo is 96x96, which should fit most of
    // your Edge Impulse projects. I don't
    // recommend going above 128x128 (yoloxl)!
    camera.config.pinout.wroom_s3();
    camera.config.pixformat.rgb();
    camera.config.resolution.yolo();

    // configure impulse
    // set minimum confidence for objects.
    // I know, it's a weird syntax, but so explicative!
    impulse.config.confidence >= 0.7;

    // init camera
    camera.begin();
    camera.raise();

    // init impulse
    impulse.begin();
    impulse.raise();

    // discard first frame which is usually bad
    camera.grab();
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

    // run impulse on frame
    if (!impulse.run(frame)) {
        Serial.println(impulse.reason());
        delay(1000);
        return;
    }

    // print detected objects coordinates
    ESP_LOGI("APP", "Found %d objects in %dms", impulse.count(), impulse.duration());

    for (uint8_t i = 0; i < impulse.count(); i++) {
        auto bbox = impulse[i];

        ESP_LOGI(
                "APP",
                "#%d) Found %s at (cx = %d, cy = %d) (size %d x %d) "
                "with confidence %.2f",
                bbox.index + 1,
                bbox.label,
                bbox.cx,
                bbox.cy,
                bbox.width,
                bbox.height,
                bbox.confidence
        );
    }
}