#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <Arduino.h>

#define DEBUG 1
#define DEBUG_LOG(...)                                       \
    do                                                       \
    {                                                        \
        if (DEBUG)                                           \
        {                                                    \
            Serial.printf("[%s: %d]: ", __FILE__, __LINE__); \
            Serial.printf(__VA_ARGS__);                      \
        }                                                    \
    } while (0)

#endif // __DEBUG_H__