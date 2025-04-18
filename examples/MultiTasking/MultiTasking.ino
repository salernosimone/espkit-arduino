/**
* Run functions in their own thread
*/
#include <espkit.h>
#include <espkit/threads.h>

String name = "Simone";

void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Multi thread example");

    // run function on core 0
    core0.pin("threadName", [](void *) {
        while (true) {
            ESP_LOGI("Thread1", "Hello world");
            delay(1000);
        }
    });

    // you can configure stack size, priority
    // and custom data to pass to function
    core0.config.stack("2 kb");
    // you can set priority from 0 (lowest) to configMAX_PRIORITIES
    // (usually configMAX_PRIORITIES is 25)
    // or use strings like: lowest, low, mid, high, highest
    // the two lines below are equivalent
    core0.config.priority(0);
    core0.config.priority("lowest");
    // always remember to cast userdata to void*
    // and be sure that the variable is available
    // in the scope of the task.
    // if we defined name here, it would be destroyed
    // at the end of setup and not be available at task time!
    String *pointer = &name;
    void *userdata = (void *) pointer;
    core0.config.userdata(pointer);
    // run
    core0.pin("anotherThreadName", [](void *userdata) {
        // convert void* back to String
        String *pointer = (String *) userdata;
        String name = *pointer;

        while (true) {
            ESP_LOGI("Thread2", "What's your name? %s", name.c_str());
            delay(2000);
        }
    });
}

void loop() {
}