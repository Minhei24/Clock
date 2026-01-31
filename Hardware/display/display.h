#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "u8g2.h"
#include "i2c.h"
#include "ds3231.h"

//菜单状态定义
#define MENU_WEEK 1
#define MENU_DATE 2
#define MENU_ALARM 3

//菜单状态变量
extern uint8_t menu_state;
//u8g2显示库
extern uint8_t num[][28*32];
extern uint8_t signal [][20*32];
extern uint8_t icon[][32*32];
extern uint8_t date[][16*16];
extern uint8_t week[][20*32];

//u8g2回调函数声明
uint8_t u8g2_stm32_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
//显示屏初始化
void Display_init(u8g2_t *u8g2);
//时间模块显示
void Display_ShowTime(u8g2_t *u8g2,DS3231_TimeTypeDef *time,DS3231_AlarmTypeDef *alarm);
//绘制模块一(显示秒,星期)
void Display_Week(u8g2_t *u8g2,DS3231_TimeTypeDef *time);
//绘制模块二(显示日期)
void Display_Date(u8g2_t *u8g2,DS3231_TimeTypeDef *time);
//绘制模块三(显示闹钟时间)
void Display_Alarm(u8g2_t *u8g2,DS3231_AlarmTypeDef *alarm);

#endif // __DISPLAY_H__