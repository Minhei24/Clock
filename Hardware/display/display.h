#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "u8g2.h"
#include "i2c.h"
#include "ds3231.h"


extern uint8_t num[][28*32];
extern uint8_t signal [][20*32];
extern uint8_t icon[][32*32];
extern uint8_t date[][16*16];

uint8_t u8g2_stm32_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
void Display_init(u8g2_t *u8g2);
void Display_ShowTime(u8g2_t *u8g2,DS3231_TimeTypeDef *time);

#endif // __DISPLAY_H__