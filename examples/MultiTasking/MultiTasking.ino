/**
* Run functions in their own thread
*/
#include <esptoolkit.h>
#include <esptoolkit/threads.h>


String name = "Simone";


void setup() {
    Serial.begin(115200);
    ESP_LOGI("APP", "Multi thread example");

    // create an "anonymous" thread
    // with default config
    threads([](void *) {
        while (true) {
            ESP_LOGI("Threads", "No config");
            delay(1000);
        }
    });

    // create a new thread
    // with every configuration available
    // (order doesn't matter)
    threads(
            [](void *userdata) {
                // convert void* back to String
                String *pointer = (String *) userdata;
                String name = *pointer;

                while (true) {
                    ESP_LOGI("Threads", "Hello %s", name.c_str());
                    delay(2000);
                }
            },
            threads.Identifier("HelloThread"),
            // pin thread to specific core
            threads.Core(0),
            // set stack size, either with int
            threads.Stack(2048),
            // ...or with string
            threads.Stack("2 kb"),
            // priority can be a value (0 is lowest)
            threads.Priority(0),
            // ...or a string (lowest, low, base, high, highest)
            threads.Priority("lowest"),
            // always remember to cast userdata to void*
            // and be sure that the variable is available
            // in the scope of the task.
            // if we defined name inside setup(), it would be destroyed
            // at the end of setup and not be available at task time!
            threads.UserData((void *) &name)
    );

    // you can also create a "detached" function that runs once
    // in background and exits (no while loop needed!)
    threads([](void *) {
        ESP_LOGI("Threads", "Run and exit!");
    });

    // you can check if the thread was created successfully
    auto created = threads([](void *) {
        ESP_LOGI("Threads", "Check if succeeded");
    });

    if (!created)
        ESP_LOGE("App", "Failed to create thread: %s", created.reason.c_str());
}

void loop() {
    // threads run in background...
}