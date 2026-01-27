#include "display.h"
#include "ds3231.h"
#include "oled.h"

// 在OLED上显示时间函数
void Display_ShowTime(DS3231_TimeTypeDef *time)
{       DS3231_GetTime(time);                   // 获取时间
        OLED_ShowNum(0, 1, 2000 + time->year, 4); // 显示年
        OLED_ShowString(0, 5, "-");              // 显示“-”符号
        OLED_ShowNum(0, 6, time->month, 2);       // 显示月
        OLED_ShowString(0, 8, "-");              // 显示“-”符号
        OLED_ShowNum(0, 9, time->day, 2);         // 显示日
        OLED_ShowString(0, 11, "-");             // 显示“-”符号
        OLED_ShowNum(0, 13, time->week, 1);       // 显示星期
        OLED_ShowNum(1, 1, time->hour, 2);        // 显示时
        OLED_ShowString(1, 3, ":");              // 显示冒号
        OLED_ShowNum(1, 4, time->min, 2);         // 显示分
        OLED_ShowString(1, 6, ":");              // 显示冒号
        OLED_ShowNum(1, 7, time->sec, 2);         // 显示秒
        HAL_Delay(100);
    }