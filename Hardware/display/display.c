#include "display.h"
#include "delay.h"
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
    u8g2_SetPowerSave(u8g2, 0); // wake up display
    u8g2_ClearDisplay(u8g2);
    //u8g2_SetFont(u8g2, u8g2_font_wqy16_t_chinese1);

}

void Display_ShowTime(u8g2_t *u8g2,DS3231_TimeTypeDef *time)
{   DS3231_GetTime(time); // 获取时间
    u8g2_DrawXBMP(u8g2,0,0,28,32,num[time->hour/10]); // 显示数字
    u8g2_DrawXBMP(u8g2,28,0,28,32,num[time->hour%10]); // 显示数字
    u8g2_DrawXBMP(u8g2,56,0,20,32,signal[0]); // 显示冒号
    u8g2_DrawXBMP(u8g2,76,0,28,32,num[time->min/10]); // 显示数字
    u8g2_DrawXBMP(u8g2,104,0,28,32,num[time->min%10]); // 显示数字
    u8g2_DrawXBMP(u8g2,0,32,32,32,icon[0]); // 显示日历图标
    u8g2_DrawXBMP(u8g2,36,32,16,16,date[2]); 
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
    u8g2_SendBuffer(u8g2);
}