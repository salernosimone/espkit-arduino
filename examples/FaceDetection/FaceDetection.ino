/**
 * Face detection.
 * Takes ~10ms when no face is present, 40-70ms when
 * at least one face is present (ESP32S3).
 *
 * Turn on INFO logging in the Tools menu to see
 * debug messages.
 *
 * Rembember to enable PSRAM in the Tools menu!
 */
#include <JPEGDEC.h>
#include <espkit.h>
#include <espkit/facekit.h>


void setup() {
    delay(3000);
    Serial.begin(115200);
    ESP_LOGI("APP", "Face Detection example");

    // configure camera
    camera.config("pin=prompt pix=jpeg q=high res=vga");

    // configure face detection
    // enable face landmarks keypoints detection
    // (more accurate but slower)
    facekit.config.keypoints(true);
    // only consider faces with confidence >= 0.6
    facekit.config.confidence >= 0.6;

    // init camera
    camera.begin();
    camera.raise();
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

    // run face detection algorithm
    facekit.detect(frame);

    if (!facekit) {
      Serial.println(facekit.reason());
      delay(1000);
      return;
    }

    // print detected faces coordinates
    ESP_LOGI("APP", "Found %d faces in %dms", facekit.count(), facekit.duration());

    for (uint8_t i = 0; i < facekit.count(); i++) {
      auto bbox = facekit[i];

      ESP_LOGI(
        "APP",
        "#%d) cx = %d, cy = %d (size %d x %d) "
        "with confidence %.2f",
        bbox.index + 1,
        bbox.cx,
        bbox.cy,
        bbox.width,
        bbox.height,
        bbox.confidence
      );

      // if keypoints are enabled, print them as JSON
      if (bbox.landmarks) {
        String landmarks = bbox.landmarks.toJSON();
        ESP_LOGI("APP", "landmarks: %s", landmarks.c_str());
      }
    }

    delay(300);
}
