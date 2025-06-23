#pragma once

#include "../support/OpStatus.h"
#include "./TaskType.h"
#include "./TaskWrapper.h"
#include "./config/Identifier.h"
#include "./config/StackSize.h"
#include "./config/Priority.h"
#include "./config/Core.h"
#include "./config/UserData.h"
#include "./config/TaskHandle.h"

using esptoolkit::OpStatus;
using esptoolkit::threads::Task;
using esptoolkit::threads::Config;
using IdentifierType = esptoolkit::threads::Identifier;
using StackSizeType = esptoolkit::threads::StackSize;
using PriorityType = esptoolkit::threads::Priority;
using CoreType = esptoolkit::threads::Core;
using UserDataType = esptoolkit::threads::UserData;
using TaskHandleType = esptoolkit::threads::TaskHandle;

namespace esptoolkit {
    class ThreadsKit {
    public:
        Config config;

        /**
         * Create thread
         * simple case: only task function
         * @param task
         */
        OpStatus operator()(Task task) {
            return handle(task, IdentifierType());
        }

        /**
         * Create thread
         * @param task
         * @return
         */
        template<typename... KWArgs>
        OpStatus operator()(Task task, KWArgs... kwargs) {
            return handle(task, kwargs...);
        }

        /**
         * Factory method for identifier config
         * @param id
         * @return
         */
        IdentifierType Identifier(const String &id) const {
            return IdentifierType(id);
        }

        /**
         * Factory method for stack size config
         * @param stack
         * @return
         */
        StackSizeType Stack(const size_t stack) const {
            return StackSizeType(stack);
        }

        /**
         * Factory method for stack size config
         * @param stack
         * @return
         */
        StackSizeType Stack(const String &stack) const {
            return StackSizeType(stack);
        }

        /**
         * Factory method for priority config
         * @param priority
         * @return
         */
        PriorityType Priority(const uint8_t priority) const {
            return PriorityType(priority);
        }

        /**
         * Factory method for priority config
         * @param priority
         * @return
         */
        PriorityType Priority(const String &priority) const {
            return PriorityType(priority);
        }

        /**
         * Factory method for core config
         * @param core
         * @return
         */
        CoreType Core(const uint8_t core) const {
            return CoreType(core);
        }

        /**
         * Factory method for user data config
         * @param userdata
         * @return
         */
        UserDataType UserData(void *userdata) const {
            return UserDataType(userdata);
        }

        /**
         * Factory method for user data config
         * @param handle
         * @return
         */
        TaskHandleType Handler(TaskHandle_t &handle) const {
            return TaskHandleType(&handle);
        }

    protected:

        /**
         * Handling stop condition
         * @param task
         */
        OpStatus handle(Task task) {
            OpStatus status;

            // actually create thread
            TaskWrapper *wrapper = new TaskWrapper(
                    config.identifier,
                    task,
                    config.userdata
            );

            ESP_LOGD("ThreadKit", "New thread: %s on core #%d", config.identifier.c_str(), config.core);

            int statusCode = xTaskCreatePinnedToCore(
                    callTask,
                    config.identifier.c_str(),
                    config.stackSize,
                    wrapper,
                    config.priority,
                    config.handler,
                    config.core
            );

            if (statusCode != pdPASS)
                return status.failWithCode("Can't pin task", statusCode);

            return status.succeed();
        }

        /**
         * Handle next argument
         * @tparam T
         * @tparam KWArgs
         * @param task
         * @param arg
         * @param kwargs
         */
        template<typename T, typename... KWArgs>
        OpStatus handle(Task task, T arg, KWArgs... kwargs) {
            arg(&config);

            return handle(task, kwargs...);
        }
    };
}

static esptoolkit::ThreadsKit threads;