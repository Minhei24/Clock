#ifndef __DISPLAY_H__
#define __DISPLAY_H__


#include "oled.h"
#include "ds3231.h"
// 在OLED上显示时间函数
void Display_ShowTime(DS3231_TimeTypeDef *time);

#endif // __DISPLAY_H__