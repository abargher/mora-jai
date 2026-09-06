#include <hardware.hpp>
#include <hardware_private.hpp>
#include <debug.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include <pins.h>

/* For OS only, do not call from user code */
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

/* For OS only, do not call from user code */
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

int _RegisterCallback(volatile BUTTON_CALLBACK *callback_set, SemaphoreHandle_t lock, BUTTON_CALLBACK handler)
{
    xSemaphoreTake(lock, portMAX_DELAY);
    for (uint32_t i = 0; i < MAX_BTN_CALLBACKS; i++)
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

int _UnregisterCallback(volatile BUTTON_CALLBACK *callback_set, SemaphoreHandle_t lock, uint32_t id)
{
    if (id < 0 || id >= MAX_BTN_CALLBACKS)
    {
        DEBUG_LOG("ID %d is not a valid callback ID.\n", id);
        return INVALID_CALLBACK_ID;
    }

    xSemaphoreTake(lock, portMAX_DELAY);
    callback_set[id] = NULL;
    xSemaphoreGive(lock);

    return EXIT_SUCCESS;
}

int RegisterButtonDownCallback(BUTTON_CALLBACK handler)
{
    return _RegisterCallback(btn_down_callbacks, btn_down_callback_lock_handle, handler);
}

int RegisterButtonUpCallback(BUTTON_CALLBACK handler)
{
    return _RegisterCallback(btn_up_callbacks, btn_up_callback_lock_handle, handler);
}

int UnregisterButtonDownCallback(uint32_t id)
{
    return _UnregisterCallback(btn_down_callbacks, btn_down_callback_lock_handle, id);
}

int UnregisterButtonUpCallback(uint32_t id)
{
    return _UnregisterCallback(btn_up_callbacks, btn_up_callback_lock_handle, id);
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

uint32_t GetBatteryMilliVolts()
{
    uint32_t raw_reading = analogReadMilliVolts(BATT_V_PIN);
    uint32_t batt_mv = raw_reading * (BATT_V_R1_VALUE + BATT_V_R2_VALUE) / BATT_V_R2_VALUE;
    DEBUG_LOG("raw battery reading: %lumV\n", raw_reading);
    DEBUG_LOG("battery voltage reading: %lumV\n", batt_mv);
    return batt_mv;
}

uint32_t GetBatteryPercentage()
{
    // uint32_t batt_mv = GetBatteryMilliVolts();
    // TODO: experiment with readings, see what range we get for real battery
    // Perhaps do some logging to a file on flash and determine ideal range
    // that way?
}

// TODO: can we set an interrupt on PWR_GOOD to detect cable plug/unplug?
BatteryState_t GetBatteryState()
{
    int pwr_good_val = digitalRead(PWR_GOOD_PIN);

    // PWR_GOOD is high, then no cable is plugged in.
    if (pwr_good_val == HIGH)
    {
        return NO_CABLE;
    }

    int charging_on_val = digitalRead(CHARG_ON_PIN);
    int charging_done_val = digitalRead(CHARG_DONE_PIN);

    // PWR_GOOD is low, but nothing else is, then we have no battery.
    // TODO: check behavior above with datasheet
    if (charging_on_val == HIGH && charging_done_val == HIGH)
    {
        return CABLE_ONLY;
    }
    // STAT1 is low, then battery is currently charging.
    else if (charging_on_val == LOW && charging_done_val == HIGH)
    {
        return CHARGING;
    }
    // STAT2 is low, then battery has finished charging.
    else if (charging_on_val == HIGH && charging_done_val == LOW)
    {
        return CHARGE_DONE;
    }
    else
    {
        return ERROR;
    }
}
