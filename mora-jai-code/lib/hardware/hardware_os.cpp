#include <hardware.hpp>
#include <hardware_private.hpp>
#include <debug.h>
#include <FreeRTOS.h>
#include <semphr.h>

void SetupEventLocks()
{
    btn_down_callback_lock_handle = xSemaphoreCreateMutexStatic(&btn_down_callback_lock);
    if (btn_down_callback_lock_handle == NULL)
    {
        DEBUG_LOG("Could not allocate memory for btn_down_callback_lock mutex\n");
    }
    btn_up_callback_lock_handle = xSemaphoreCreateMutexStatic(&btn_up_callback_lock);
    if (btn_up_callback_lock_handle == NULL)
    {
        DEBUG_LOG("Could not allocate memory for btn_up_callback_lock mutex\n");
    }
}

void ExecuteCallbacks(buttonUpdate_t data)
{
    volatile BUTTON_CALLBACK *callbacks;
    SemaphoreHandle_t lock;
    if (data.isPressed)
    {
        callbacks = btn_down_callbacks;
        lock = btn_down_callback_lock_handle;
    }
    else
    {
        callbacks = btn_up_callbacks;
        lock = btn_up_callback_lock_handle;
    }

    for (uint32_t i = 0; i < MAX_BTN_CALLBACKS; i++)
    {
        BUTTON_CALLBACK callback = NULL;
        xSemaphoreTake(lock, portMAX_DELAY);
        callback = callbacks[i];
        xSemaphoreGive(lock);
        if (callback != NULL)
        {
            callback(data);
        }
    }
}
