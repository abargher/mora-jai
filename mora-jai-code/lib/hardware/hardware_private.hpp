#ifndef __HARDWARE_PRIVATE_H__
#define __HARDWARE_PRIVATE_H__
#include <ESP32Servo.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include <hardware.hpp>

#define LATCH_STATE_FILEPATH "/lockstate.txt"
#define LATCH_LOCKED_ANGLE 90
#define LATCH_UNLOCKED_ANGLE 0

#define MAX_BTN_CALLBACKS 5

#define NO_FREE_CALLBACK_SLOT -1
#define INVALID_CALLBACK_ID -1

Servo servo;

SemaphoreHandle_t btn_up_callback_lock_handle;
SemaphoreHandle_t btn_down_callback_lock_handle;
StaticSemaphore_t btn_up_callback_lock;
StaticSemaphore_t btn_down_callback_lock;
volatile BUTTON_CALLBACK btn_up_callbacks[MAX_BTN_CALLBACKS] = {NULL};
volatile BUTTON_CALLBACK btn_down_callbacks[MAX_BTN_CALLBACKS] = {NULL};

#endif // __HARDWARE_PRIVATE_H__