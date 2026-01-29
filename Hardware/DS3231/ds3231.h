#ifndef __DS3231_H__
#define __DS3231_H__


#include "i2c.h"
#include "main.h"

// DS3231的I2C地址（固定）
#define DS3231_ADDR    0xD0  // DS3231 I2C地址

// DS3231寄存器定义
#define DS3231_SEC     0x00  // 秒寄存器
#define DS3231_MIN     0x01  // 分寄存器
#define DS3231_HOUR    0x02  // 时寄存器
#define DS3231_WEEK    0x03  // 星期寄存器
#define DS3231_DAY     0x04  // 日寄存器
#define DS3231_MONTH   0x05  // 月寄存器
#define DS3231_YEAR    0x06  // 年寄存器

//DS3231闹钟寄存器定义
#define ALARM1_SEC     0x07  // Alarm1秒寄存器
#define ALARM1_MIN     0x08  // Alarm1分寄存器
#define ALARM1_HOUR    0x09  // Alarm1时寄存器
#define ALARM1_DAY     0x0A  // Alarm1日寄存器
#define ALARM2_MIN     0x0B  // Alarm2分寄存器
#define CONTROL_REG    0x0E  // 控制寄存器
#define STATUS_REG     0x0F  // 状态寄存器

// 时间结构体
typedef struct {
    uint8_t year;   // 年（0-99，对应2000-2099）
    uint8_t month;  // 月（1-12）
    uint8_t day;    // 日（1-31）
    uint8_t week;   // 星期（1-7，1=周一，7=周日）
    uint8_t hour;   // 时（0-23）
    uint8_t min;    // 分（0-59）
    uint8_t sec;    // 秒（0-59）
} DS3231_TimeTypeDef;

//闹钟状态结构体
typedef struct {
    uint8_t Alarm1Flag; // Alarm1标志
    uint8_t ahour; // Alarm1小时
    uint8_t amin;  // Alarm1分钟
    uint8_t asec;  // Alarm1秒钟
} DS3231_AlarmTypeDef;


// 函数声明
// BCD码转十进制
uint8_t BCD2DEC(uint8_t bcd);
// 十进制转BCD码
uint8_t DEC2BCD(uint8_t dec);
// 设置DS3231时间
void DS3231_SetTime(uint8_t year, uint8_t month, uint8_t day, uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);
// 读取DS3231时间
void DS3231_GetTime(DS3231_TimeTypeDef *time);
// 设置DS3231Alarm1（每天在指定时间触发）
void DS3231_SetAlarm1(uint8_t hour, uint8_t min, uint8_t sec);
// 禁用DS3231Alarm1
void DS3231_DisableAlarm1(void);
// 仅关闭本次Alarm1（下次仍会触发）
void DS3231_CloseAlarm(void);
//读取Alarm1时间
void DS3231_ReadAlarm1Time(DS3231_AlarmTypeDef *alarm);


#endif // __DS3231_H__