#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "u8g2.h"
#include "i2c.h"
#include "ds3231.h"

static uint8_t menu_state = 1;//菜单状态变量
extern uint8_t num[][28*32];
extern uint8_t signal [][20*32];
extern uint8_t icon[][32*32];
extern uint8_t date[][16*16];

uint8_t u8g2_stm32_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);//u8g2回调函数声明
void Display_init(u8g2_t *u8g2);//显示屏初始化
void Display_ShowTime(u8g2_t *u8g2,DS3231_TimeTypeDef *time,DS3231_AlarmTypeDef *alarm);//时间模块显示
void Display_Date(u8g2_t *u8g2,DS3231_TimeTypeDef *time);//绘制日期模块
void Display_Alarm(u8g2_t *u8g2,DS3231_AlarmTypeDef *alarm);//绘制闹钟模块
#endif // __DISPLAY_H__