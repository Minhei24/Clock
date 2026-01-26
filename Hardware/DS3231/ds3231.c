#include "ds3231.h"
// BCD码转十进制
uint8_t BCD2DEC(uint8_t bcd) 
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

// 十进制转BCD  码
uint8_t DEC2BCD(uint8_t dec) 
{
    return ((dec / 10) << 4) + (dec % 10);
}

// 设置DS3231时间
HAL_StatusTypeDef DS3231_SetTime(DS3231_TimeTypeDef *time) 
{
    uint8_t buf[7];
    // 按寄存器顺序填充：秒、分、时、星期、日、月、年
    buf[0] = DEC2BCD(time->sec);
    buf[1] = DEC2BCD(time->min);
    buf[2] = DEC2BCD(time->hour);
    buf[3] = DEC2BCD(time->week);
    buf[4] = DEC2BCD(time->day);
    buf[5] = DEC2BCD(time->month);
    buf[6] = DEC2BCD(time->year);
    
    // 从秒寄存器开始，连续写入7个字节
    return HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDR, DS3231_SEC, I2C_MEMADD_SIZE_8BIT, buf, 7, 500);
}

// 读取DS3231时间
HAL_StatusTypeDef DS3231_GetTime(DS3231_TimeTypeDef *time) 
{
    uint8_t buf[7];
    // 从秒寄存器开始，连续读取7个字节
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDR, DS3231_SEC, I2C_MEMADD_SIZE_8BIT, buf, 7, 500);
    if (ret == HAL_OK) {
        // BCD转十进制，赋值给时间结构体
        time->sec = BCD2DEC(buf[0]);
        time->min = BCD2DEC(buf[1]);
        time->hour = BCD2DEC(buf[2]);
        time->week = BCD2DEC(buf[3]);
        time->day = BCD2DEC(buf[4]);
        time->month = BCD2DEC(buf[5]);
        time->year = BCD2DEC(buf[6]);
    }
    
    return ret;
}