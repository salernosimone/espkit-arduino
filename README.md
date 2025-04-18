# EspKit

A library of useful, carefully crafted components for your Arduino project on ESP32.


## Install

The library is available in the Library Manager. Be sure you always install the latest version.


## Examples

The full list of examples is in the `examples` folder. Here are some extracts to give you a glimpse of the components.


### Capture camera frames

```cpp
// configure camera
camera.config.pinout.prompt();
camera.config.pixformat.jpeg();
camera.config.quality.high();
camera.config.resolution.qvga();
camera.begin();

// grab frame
auto frame = camera.grab();

if (!frame)
    return;

Serial.printf("Frame size: %d bytes\n", frame.size());
```

### Run Edge Impulse FOMO model

```cpp
// configure camera
camera.config.pinout.wroom_s3();
camera.config.pixformat.rgb();
camera.config.resolution.yolo();
camera.begin();

// configure impulse
impulse.config.confidence >= 0.7;
impulse.begin();

// run FOMO on frame
auto frame = camera.grab();

impulse.run(frame);

for (uint8_t i = 0; i < impulse.count(); i++) {
    auto bbox = impulse[i];

    Serial.printf(
        "#%d) Found %s at (cx = %d, cy = %d) (size %d x %d) "
        "with confidence %.2f\n",
        bbox.index + 1,
        bbox.label,
        bbox.cx,
        bbox.cy,
        bbox.width,
        bbox.height,
        bbox.confidence
    );
}
```

### Run multiple tasks

```cpp
// run function on core 0
core0.pin("threadName", [](void *) {
    while (true) {
        ESP_LOGI("Thread1", "Hello world");
        delay(1000);
    }
});


// you can configure stack size, priority
// and custom data to pass to function
String name = "Simone";
String *pointer = &name;
void *userdata = (void *) pointer;

core0.config.stack("2 kb");
core0.config.priority("lowest");
core0.config.userdata(pointer);

core0.pin("anotherThreadName", [](void *userdata) {
    // convert void* back to String
    String *pointer = (String *) userdata;
    String name = *pointer;

    while (true) {
        ESP_LOGI("Thread2", "What's your name? %s", name.c_str());
        delay(2000);
    }
});

```