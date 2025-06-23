#pragma once

#include <functional>

using Task = std::function<void(void *)>;

/**
 * Wrap a C-style function that gets passed to xTaskCreatePinnedToCore
 */
struct TaskWrapper {
    String identifier;
    Task task;
    void *param;

    TaskWrapper(const String &id, Task t, void *p) :
            identifier(id),
            task(t),
            param(p) {}
};


/**
 * Allow the use of a std::function as a C-style function for xTaskCreatePinnedToCore
 */
extern "C" void callTask(void *arg) {
    TaskWrapper *wrapper = static_cast<TaskWrapper *>(arg);

    wrapper->task(wrapper->param);
    vTaskDelete(NULL);

    delete wrapper;
}