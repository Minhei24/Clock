#include "display.h"
#include "delay.h"

uint8_t menu_state = 1;//菜单状态变量，默认显示日期模块


uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {

    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        break;

    case U8X8_MSG_DELAY_MILLI:
        delay_ms(arg_int);
        break;

    default:
        return 0;
    }
    return 1; // command processed successfully.
}

void Display_init(u8g2_t *u8g2)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8g2_gpio_and_delay_stm32);
    u8g2_InitDisplay(u8g2);     // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(u8g2, 0); // 省电模式关闭
    u8g2_ClearDisplay(u8g2);
}

void Display_ShowTime(u8g2_t *u8g2,DS3231_TimeTypeDef *time,DS3231_AlarmTypeDef *alarm)//时间模块显示
{   DS3231_GetTime(time); // 获取时间
    DS3231_ReadAlarm1Time(alarm); // 获取闹钟时间
    u8g2_DrawXBMP(u8g2,0,0,28,32,num[time->hour/10]); 
    u8g2_DrawXBMP(u8g2,28,0,28,32,num[time->hour%10]); // 显示小时
    u8g2_DrawXBMP(u8g2,56,0,20,32,signal[0]); // 显示冒号
    u8g2_DrawXBMP(u8g2,76,0,28,32,num[time->min/10]); 
    u8g2_DrawXBMP(u8g2,104,0,28,32,num[time->min%10]); // 显示小时
    switch (menu_state)
    {
    case MENU_DATE:
        u8g2_SetDrawColor(u8g2,0);
        u8g2_DrawBox(u8g2,0,32,128,32);
        u8g2_SetDrawColor(u8g2,1);
        Display_Date(u8g2,time);
        break;
    case MENU_ALARM:
        u8g2_SetDrawColor(u8g2,0);
        u8g2_DrawBox(u8g2,0,32,128,32);
        u8g2_SetDrawColor(u8g2,1);
        Display_Alarm(u8g2,alarm);
        break;
    case MENU_WEEK:
        u8g2_SetDrawColor(u8g2,0);
        u8g2_DrawBox(u8g2,0,32,128,32);
        u8g2_SetDrawColor(u8g2,1);
        Display_Week(u8g2,time);
    default:
        break;
    }
    u8g2_SendBuffer(u8g2);
}

void Display_Date(u8g2_t *u8g2,DS3231_TimeTypeDef *time)//绘制模块二(显示日期)
{
    u8g2_DrawXBMP(u8g2,0,32,32,32,icon[0]); // 显示日历图标
    u8g2_DrawXBMP(u8g2,34,32,16,16,date[2]); 
    u8g2_DrawXBMP(u8g2,54,32,16,16,date[0]);
    u8g2_DrawXBMP(u8g2,72,32,16,16,date[time->year/10]); 
    u8g2_DrawXBMP(u8g2,92,32,16,16,date[time->year%10]); 
    u8g2_DrawXBMP(u8g2,112,32,16,16,date[10]); // 显示年份
    u8g2_DrawXBMP(u8g2,32,48,16,16,date[time->month/10]); 
    u8g2_DrawXBMP(u8g2,48,48,16,16,date[time->month%10]); 
    u8g2_DrawXBMP(u8g2,64,48,16,16,date[11]); // 显示月份
    u8g2_DrawXBMP(u8g2,80,48,16,16,date[time->day/10]); 
    u8g2_DrawXBMP(u8g2,96,48,16,16,date[time->day%10]); 
    u8g2_DrawXBMP(u8g2,112,48,16,16,date[12]); // 显示日期
}

void Display_Alarm(u8g2_t *u8g2,DS3231_AlarmTypeDef *alarm)//绘制模块三(显示闹钟信息)
{
    u8g2_DrawXBMP(u8g2,32,32,16,16,date[14]); // 显示闹钟文字
    u8g2_DrawXBMP(u8g2,50,32,16,16,date[15]);   
    u8g2_DrawXBMP(u8g2,68,32,16,16,date[14]);
    u8g2_DrawXBMP(u8g2,90,32,16,16,date[16]);
    u8g2_DrawXBMP(u8g2,112,32,16,16,date[17]);
    u8g2_DrawXBMP(u8g2,0,32,32,32,icon[1]); // 显示闹钟图标
    u8g2_DrawXBMP(u8g2,34,48,16,16,date[alarm->ahour/10]); 
    u8g2_DrawXBMP(u8g2,54,48,16,16,date[alarm->ahour%10]);
    u8g2_DrawXBMP(u8g2,72,48,16,16,date[13]); // 显示冒号
    u8g2_DrawXBMP(u8g2,94,48,16,16,date[alarm->amin/10]); 
    u8g2_DrawXBMP(u8g2,112,48,16,16,date[alarm->amin%10]);
    
}

void Display_Week(u8g2_t *u8g2,DS3231_TimeTypeDef *time)//绘制模块一(显示秒,星期)
{   
    DS3231_GetTime(time);
    u8g2_DrawXBMP(u8g2,0,32,28,32,num[time->sec/10]);
    u8g2_DrawXBMP(u8g2,28,32,28,32,num[time->sec%10]);
    u8g2_DrawXBMP(u8g2,64,32,20,32,signal[1]);
    u8g2_DrawXBMP(u8g2,84,32,20,32,signal[2]);
    u8g2_DrawXBMP(u8g2,108,32,20,32,week[time->week-1]);
}