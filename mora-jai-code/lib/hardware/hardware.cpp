#include <hardware.hpp>
#include <hardware_private.hpp>
#include <debug.h>
#include <FreeRTOS.h>
#include <semphr.h>

void SetupEvents()
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

int _RegisterCallback(volatile BUTTON_CALLBACK *callback_set, SemaphoreHandle_t lock, BUTTON_CALLBACK handler)
{
    xSemaphoreTake(lock, portMAX_DELAY);
    for (int i = 0; i < MAX_BTN_CALLBACKS; i++)
    {
        if (callback_set[i] == NULL)
        {
            callback_set[i] = handler;
            xSemaphoreGive(lock);
            return i;
        }
    }
    xSemaphoreGive(lock);

    DEBUG_LOG("No free callback slot found.\n");
    return NO_FREE_CALLBACK_SLOT;
}

int RegisterButtonDownCallback(volatile BUTTON_CALLBACK handler)
{
    return _RegisterCallback(btn_down_callbacks, btn_down_callback_lock_handle, handler);
}

int RegisterButtonUpCallback(BUTTON_CALLBACK handler)
{
    return _RegisterCallback(btn_up_callbacks, btn_up_callback_lock_handle, handler);
}

void SetRGBMatrix(uint32_t *colors)
{
}

void SetRGBSingle(uint32_t index, uint32_t color)
{
}

void SetColorMatrix(BtnColor_t *colors)
{
    return SetRGBMatrix((uint32_t *)colors);
}

void SetColorSingle(uint32_t index, BtnColor_t color)
{
    return SetRGBSingle(index, (uint32_t)color);
}

void LatchUnlock()
{
    /* Check latch state file, if unlocked, do nothing.

    If locked, move servo to unlock position and update file.
    */
}

void LatchLock()
{
    /* Check latch state file, if locked, do nothing.

    If unlocked, move servo to lock position and update file.
    */
}
